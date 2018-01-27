package Server;

import org.java_websocket.WebSocket;
import proto.ChatMessageProto.ChatMessage;
import proto.FriendListMessageProto.FriendListMessage;
import proto.MessageProto.Message;

import java.sql.SQLException;
import java.util.ArrayList;

public class FriendListMessageHandler extends Thread {
    private DBManager dbManager;
    private WebSocket webSocket;
    private Session session;
    private Iterable<? extends FriendListMessage> friendListMessages;

    private FriendListMessageHandler(DBManager dbManager, WebSocket webSocket, Session session, Iterable<? extends FriendListMessage> friendListMessages) {
        this.dbManager = dbManager;
        this.webSocket = webSocket;
        this.session = session;
        this.friendListMessages = friendListMessages;
    }

    @Override
    public void run() {
        for(FriendListMessage friendListMessage : friendListMessages){
            handle(friendListMessage);
        }
    }

    static void handle(DBManager dbManager, WebSocket webSocket, Session session, Iterable<? extends FriendListMessage> friendListMessages) {
        new FriendListMessageHandler(dbManager, webSocket, session, friendListMessages).start();
    }

    private void addFriend(long userID, long friendID) {
        Message.Builder builder = Message.newBuilder();
        try {
            FriendListMessage message = dbManager.addFriend(userID, friendID);
            if(message != null){
                builder.setType(Message.Type.FriendListMessage);
                builder.addFriendListMesage(message);
                session.sendTo(friendID, Message.newBuilder().setType(Message.Type.FriendListMessage).addFriendListMesage(
                        dbManager.getProfile(userID, FriendListMessage.Type.FRIEND)
                ).build().toByteArray());
            }
            else {
                builder.setType(Message.Type.ErrorMessage);
                builder.setErrorMessage("Add friend error.");
            }
        } catch (SQLException e) {
            builder.setType(Message.Type.ErrorMessage);
            builder.setErrorMessage(e.getMessage());
        } finally {
            webSocket.send(builder.build().toByteArray());
        }
    }

    private void getFriendList(long userID) {
        Message.Builder builder = Message.newBuilder();
        try {
            ArrayList<FriendListMessage> friendList = dbManager.getFriendList(userID);
            ArrayList<FriendListMessage> groupList = dbManager.getGourpList(userID);
            FriendListMessage profile = dbManager.getProfile(userID, FriendListMessage.Type.SELF);
            builder.addAllFriendListMesage(friendList);
            builder.addAllFriendListMesage(groupList);
            builder.addFriendListMesage(profile);
            builder.setType(Message.Type.FriendListMessage);
        } catch (SQLException e) {
            builder.setType(Message.Type.ErrorMessage);
            builder.setErrorMessage(e.getMessage());
        } finally {
            webSocket.send(builder.build().toByteArray());
        }
    }

    private void handle(FriendListMessage friendListMessage){
        if(!friendListMessage.hasType() || webSocket.getAttachment() == null || !(webSocket.getAttachment() instanceof Long))
            return;
        long account = webSocket.getAttachment();
        switch (friendListMessage.getType()) {
            case REQUIRE:
                if(webSocket.getAttachment() != null && webSocket.getAttachment() instanceof Long) {
                    getFriendList(account);
                    getChatMessage(account);
                }
                break;
            case ADDFRIEND:
                if(friendListMessage.hasAccount())
                    addFriend(account, friendListMessage.getAccount());
                break;
            case ADDGROUP:
                break;
            default:
        }
    }

    private void getChatMessage(long account) {
        try {
            Message.Builder builder = Message.newBuilder();
            ArrayList<ChatMessage> chatMessages = dbManager.getUserChatRecord(account);
            builder.setType(Message.Type.ChatMessage);
            builder.addAllChatMessage(chatMessages);
            webSocket.send(builder.build().toByteArray());
            dbManager.resetChatRecord(account);
        } catch (SQLException e) {
            //ignore
        }
    }

}
