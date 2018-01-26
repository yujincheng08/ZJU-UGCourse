package Server;

import org.java_websocket.WebSocket;

import java.util.HashMap;

public class Session {
    private HashMap<Long, WebSocket> sessions = new HashMap<>();
    void login(long user, WebSocket webSocket) {
        if(sessions.containsKey(user)) {
            WebSocket old = sessions.get(user);
            if(old.isOpen())
                sessions.get(user).close(1000, "Login else where");
        }
        sessions.put(user, webSocket);
        webSocket.setAttachment(user);
    }

    void logout(long user){
        sessions.get(user).close(1, "Logout");
        sessions.remove(user);
    }

    void sendTo(long user, byte [] message){
        if(sessions.containsKey(user)) {
            new Thread(()->sessions.get(user).send(message)).run();
        }
        else
           ; //DBManager.store(user, message);
    }

}
