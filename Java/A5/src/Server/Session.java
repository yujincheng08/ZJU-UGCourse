package Server;

import org.java_websocket.WebSocket;

import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;

public class Session {
    private ConcurrentHashMap<Long, WebSocket> sessions = new ConcurrentHashMap<>();
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

    boolean sendTo(long user, byte [] message){
        if(isOnline(user)) {
            sessions.get(user).send(message);
            return true;
        }
        else
            return false;
    }

    boolean isOnline(long user) {
        return sessions.containsKey(user) && sessions.get(user).isOpen();
    }


}
