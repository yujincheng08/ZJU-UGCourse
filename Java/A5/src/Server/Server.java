package Server;

import com.google.protobuf.InvalidProtocolBufferException;
import org.java_websocket.WebSocket;
import org.java_websocket.WebSocketImpl;
import org.java_websocket.drafts.Draft;
import org.java_websocket.drafts.Draft_6455;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;
import proto.ChatMessageProto;
import proto.LoginMessageProto.LoginMessage;
import proto.MessageProto.Message;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.sql.SQLException;
import java.util.Collections;

public class Server extends WebSocketServer{
    private static int clientCount = 0;
    private Session session = new Session();
    private DBManager dbManager;

    Server(int port, Draft d) {
        super(new InetSocketAddress(port), Collections.singletonList(d));
        try {
            dbManager = new DBManager("server.db");
        } catch (SQLException e){
            e.printStackTrace();
        }
    }

    Server(InetSocketAddress inetSocketAddress, Draft d) {
        super(inetSocketAddress, Collections.singletonList(d));
        try {
            dbManager = new DBManager("server.db");
        } catch (SQLException e){
            e.printStackTrace();
        }
    }


    @Override
    public void onOpen(WebSocket webSocket, ClientHandshake clientHandshake) {
        clientCount++;
        System.out.println("New client " + webSocket.getRemoteSocketAddress() + " connected. Totally " + clientCount);
        clientHandshake.getResourceDescriptor();
    }

    @Override
    public void onClose(WebSocket webSocket, int i, String s, boolean b) {
        clientCount--;
        System.out.println("Client disconnected. Totally " + clientCount);

    }

    @Override
    public void onMessage(WebSocket webSocket, String s) {
        System.out.println("Get: " + s);
        webSocket.send(s);
    }

    private boolean login(long userID, String password)
            throws SQLException {
        return dbManager.login(userID, password);
    }

    private void webSocketSend(WebSocket conn, LoginMessage loginMessage){
        if(loginMessage == null)
            return;
        Message.Builder builder = Message.newBuilder();
        builder.setType(Message.Type.LoginMessage);
        builder.setLoginMessage(loginMessage);
        conn.send(builder.build().toByteArray());
    }

    private LoginMessage login(WebSocket webSocket, LoginMessage loginMessage) {
        String prompt = "";
        boolean success = false;
        try {
            if (!loginMessage.hasAccount())
                prompt = "Account cannot be empty.";
            else if (!loginMessage.hasPassword())
                prompt = "Password cannot be empty.";
            else if (!login(loginMessage.getAccount(), loginMessage.getPassword()))
                prompt = "Account and password do not match.";
            else
                success = true;
        }catch (SQLException e) {
            prompt = e.getMessage();
        }
        LoginMessage.Builder builder = LoginMessage.newBuilder();
        builder.setType(LoginMessage.Type.LOGIN);
        if (!prompt.isEmpty())
            builder.setPrompt(prompt);
        builder.setStatus(success ? LoginMessage.Status.SUCCESS : LoginMessage.Status.FAIL);
        builder.setAccount(loginMessage.getAccount());
        if(success) session.login(loginMessage.getAccount(), webSocket);
        return builder.build();
    }

    private long register(String password, String nickName, String email)
        throws SQLException {
        return dbManager.register(password, nickName, email);
    }

    private LoginMessage register(LoginMessage loginMessage) {
        String prompt = "";
        long key = -1;
        try{
            if (!loginMessage.hasPassword())
                prompt = "Password cannot be empty";
            else if(!loginMessage.hasEmail())
                prompt = "Email address cannot be empty";
            else if(!loginMessage.hasNickName())
                prompt = "Nick name cannot be empty";
            else
                key = register(loginMessage.getPassword(), loginMessage.getNickName(), loginMessage.getEmail());
        }catch (SQLException e) {
            prompt = e.getMessage();
        }
        LoginMessage.Builder builder = LoginMessage.newBuilder();
        builder.setType(LoginMessage.Type.REGISTER);
        builder.setAccount(key);
        if (!prompt.isEmpty())
            builder.setPrompt(prompt);
        builder.setStatus(key > -1 ? LoginMessage.Status.SUCCESS : LoginMessage.Status.FAIL);
        return builder.build();
    }

    private LoginMessage handlerLoginMessage(WebSocket webSocket, LoginMessage loginMessage) {
        if(loginMessage.hasType()) {
            switch(loginMessage.getType())
            {
                case LOGIN:
                    return login(webSocket, loginMessage);
                case REGISTER:
                    return register(loginMessage);
            }
        }
        return null;
    }

    @Override
    public void onMessage(WebSocket conn, ByteBuffer bytes) {
        System.out.println(bytes);
        try{
            Message message = Message.parseFrom(bytes.array());
            if(message.hasType())
            {
                switch (message.getType()) {
                    case LoginMessage:
                        if(message.hasLoginMessage())
                            webSocketSend(conn, handlerLoginMessage(conn, message.getLoginMessage()));
                        break;

                    case FriendListMessage:
                        FriendListMessageHandler.handle(dbManager, conn, session, message.getFriendListMesageList());
                        break;
                    case ChatMessage:
                            ChatMessageHandler.handle(dbManager, conn, session, message.getChatMessageList());
                        break;
                }
            }

        }catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onError(WebSocket webSocket, Exception e) {
        System.out.println("Error: ");
        e.printStackTrace();
    }

    @Override
    public void onStart() {
        System.out.println("Server started!");
    }

    public static void main(String[] args) {
        WebSocketImpl.DEBUG = false;
        int port;
        try {
            port = new Integer(args[0]);
        } catch (Exception e) {
            System.out.println("No port specified, using default: 9000");
            port = 9000;
        }

        Server server = new Server(port, new Draft_6455());
        server.setConnectionLostTimeout(3000);
        server.start();
    }
}
