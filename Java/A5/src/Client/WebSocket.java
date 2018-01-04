package Client;

import javafx.application.Platform;
import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;
import java.nio.ByteBuffer;

public class WebSocket extends WebSocketClient{
    WebSocketHandler handler = null;
    WebSocket(URI serverUri , Draft draft ) {
        super(serverUri, draft);
    }

    WebSocket(URI serverUri) {
        super(serverUri);
    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {
        if(handler != null)
            Platform.runLater(() -> handler.onOpen(serverHandshake));
    }

    @Override
    public void onMessage(String s) {
        if(handler != null)
            Platform.runLater(()->handler.onMessage(s));
    }

    @Override
    public void onMessage(ByteBuffer message)
    {
        if(handler != null)
            Platform.runLater(()->handler.onMessage(message));
    }

    @Override
    public void onClose(int i, String s, boolean b) {
        if(handler != null)
            Platform.runLater(()->handler.onClose(i, s, b));
    }

    @Override
    public void onError(Exception e) {
        if(handler != null)
            Platform.runLater(() -> handler.onError(e));
    }

    public void setHandler(WebSocketHandler handler) {
        this.handler = handler;
    }

    public WebSocketHandler getHander() {
        return handler;
    }


}
