package Client;

import org.java_websocket.handshake.ServerHandshake;

public class MainWindowController implements StageController, WebSocketHandler
{
    private MainController mainController;
    private String stageName;
    @Override
    public void setMainController(MainController mainController, String currentStageName)
    {
        this.mainController = mainController;
        this.stageName = currentStageName;
    }

    @Override
    public void onError(Exception exception) {

    }

    @Override
    public void onMessage(String message) {

    }

    @Override
    public void onClose(int code, String reason, boolean remote) {

    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }
}
