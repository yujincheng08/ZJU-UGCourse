package Server;

import org.java_websocket.WebSocket;
import org.java_websocket.WebSocketImpl;
import org.java_websocket.drafts.Draft;
import org.java_websocket.drafts.Draft_6455;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.util.Collections;

public class Server extends WebSocketServer{
    private static int clientCount = 0;
    private

    Server(int port, Draft d) throws UnknownHostException {
        super(new InetSocketAddress(port), Collections.singletonList(d));
    }

    Server(InetSocketAddress inetSocketAddress, Draft d) {
        super(inetSocketAddress, Collections.singletonList(d));
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

    @Override
    public void onError(WebSocket webSocket, Exception e) {
        System.out.println("Error: ");
        e.printStackTrace();
    }

    @Override
    public void onStart() {
        System.out.println("Server started!");
    }

    public static void main(String[] args) throws  UnknownHostException {
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
