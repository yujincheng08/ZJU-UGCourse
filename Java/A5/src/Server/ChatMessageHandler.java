package Server;
import org.java_websocket.WebSocket;
import proto.ChatMessageProto.ChatMessage;
import proto.MessageProto.Message;

import java.sql.SQLException;


public class ChatMessageHandler extends Thread {
    private Iterable<ChatMessage> messages;
    private DBManager dbManager;
    private long userID;
    private WebSocket webSocket;
    private Session session;
    private ChatMessageHandler(DBManager dbManager, WebSocket webSocket, Session session, Iterable<ChatMessage> messages) {
        this.messages = messages;
        this.userID = webSocket.getAttachment();
        this.dbManager = dbManager;
        this.webSocket = webSocket;
        this.session = session;
    }
    public static void handle(DBManager dbManager, WebSocket webSocket, Session session, Iterable<ChatMessage> messages) {
        new ChatMessageHandler(dbManager, webSocket, session, messages).start();
    }

    private void handleGroupMessage(ChatMessage message) {
        //long groupID = message.getGroupID();
        //dbManager.getMemberList();
    }

    private void handlePrivateMessage(ChatMessage message) {
        if(!message.hasTimestamp() || !message.hasContent())
            return;
        String content = message.getContent();
        long timestamp = message.getTimestamp();
        long fromID = userID;
        long toID = message.getUserID();
        if(!session.sendTo(toID, Message.newBuilder().
                setType(Message.Type.ChatMessage).
                addChatMessage(ChatMessage.newBuilder().
                        setTimestamp(timestamp).
                        setUserID(fromID).
                        setContent(content))
                .build().toByteArray()))
            try {
                dbManager.setUserChatRecord(fromID, toID, content, timestamp);
            }catch(SQLException e) {
                webSocket.send(Message.newBuilder().setType(Message.Type.ErrorMessage).
                    setErrorMessage(e.getMessage()).build().toByteArray());
            }
    }

    @Override
    public void run() {
        for(ChatMessage message : messages) {
            if (message.hasGroupID()) {
                handleGroupMessage(message);
            } else if (message.hasUserID()) {
                handlePrivateMessage(message);
            }
        }
    }
}
