package Client;

import proto.ChatMessageProto.ChatMessage;
import proto.LoginMessageProto.LoginMessage;
import proto.MessageProto.Message;
import proto.FriendListMessageProto.FriendListMessage;

import java.net.URI;
import java.net.URISyntaxException;
import java.sql.SQLException;

public class MainController extends StagesController
{

    private WebSocket webSocket;
    private URI uri;
    private DBController dbController;

    MainController(int port)
    {
        try {
            uri = new URI("ws://127.0.0.1:" + port);
        }catch(URISyntaxException e)
        {
            e.printStackTrace();
        }
    }

    public void setWebSocketHandler(WebSocketHandler webSocketHandler)
    {
        if(webSocket == null) {
            webSocket = new WebSocket(uri);
            webSocket.setHandler(webSocketHandler);
            webSocket.connect();
        }
        else
            webSocket.setHandler(webSocketHandler);
    }

    private boolean webScoketCheck()
    {
        if(webSocket.isOpen())
            return true;
        else {
            WebSocketHandler handler = webSocket.getHander();
            webSocket = new WebSocket(uri);
            webSocket.setHandler(handler);
            webSocket.connect();
            return webSocket.isOpen();
        }
    }

    public WebSocketHandler getWebSocketHandler()
    {
        if(webSocket != null)
            return webSocket.getHander();
        else
            return null;
    }

    public void webSocketSend(LoginMessage loginMessage)
    {
        if(webScoketCheck()) {
            Message.Builder messageBuilder = Message.newBuilder();
            messageBuilder.setType(Message.Type.LoginMessage);
            messageBuilder.setLoginMessage(loginMessage);
            webSocket.send(messageBuilder.build().toByteArray());
        }

    }

    public void webSocketSend(ChatMessage chatMessage)
    {
        if(webScoketCheck()) {
            Message.Builder messageBuilder = Message.newBuilder();
            messageBuilder.setType(Message.Type.ChatMessage);
            messageBuilder.addChatMessage(chatMessage);
            webSocket.send(messageBuilder.build().toByteArray());
        }
    }

    public void webSocketSend(FriendListMessage friendListMessage){
        if(webScoketCheck()) {
            Message.Builder messageBuilder = Message.newBuilder();
            messageBuilder.setType(Message.Type.FriendListMessage);
            messageBuilder.addFriendListMesage(friendListMessage);
            webSocket.send(messageBuilder.build().toByteArray());
        }
    }

    public void webSocketSend(Iterable<? extends FriendListMessage> friendListMessages) {
        if(webScoketCheck()){
            Message.Builder messageBuilder = Message.newBuilder();
            messageBuilder.setType(Message.Type.FriendListMessage);
            messageBuilder.addAllFriendListMesage(friendListMessages);
            webSocket.send(messageBuilder.build().toByteArray());
        }
    }

    @Override
    public void stageClosed(String name){
        if(shownStageCount == 0)
            webSocket.close();
    }

    public void setDbController(long userID) throws SQLException {
        dbController = new DBController(userID+".db");
    }

    public DBController getDbController() {
        return dbController;
    }
}
