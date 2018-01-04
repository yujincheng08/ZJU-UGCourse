package Client;

import org.java_websocket.handshake.ServerHandshake;

import java.nio.ByteBuffer;

public interface WebSocketHandler {
    default void onError( Exception exception ){}
    default void onMessage(String message){}
    default void onMessage(ByteBuffer message){}
    default void onClose(int code, String reason, boolean remote){}
    default void onOpen(ServerHandshake serverHandshake){}
}
