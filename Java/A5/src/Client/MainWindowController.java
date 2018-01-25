package Client;

import Client.FriendListItem.FriendListItem;
import Client.FriendListItem.FriendListItemModel;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.ListView;
import javafx.stage.WindowEvent;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URL;
import java.util.ResourceBundle;

public class MainWindowController implements StageController, WebSocketHandler, Initializable {
    @FXML
    private ListView groupListView;
    @FXML
    private ListView<FriendListItemModel> friendListView;
    private WebSocketHandler backupHandler;
    private MainController mainController;
    private String stageName;

    @Override
    public void setMainController(MainController mainController, String currentStageName) {
        this.mainController = mainController;
        this.stageName = currentStageName;
    }

    @Override
    public void onError(Exception exception) {

    }

    @Override
    public void onMessage(String message) {

    }

    public void onClose(int code, String reason, boolean remote) {
        System.out.println(reason);
        mainController.setWebSocketHandler(backupHandler);
        AlertHelper.show("Connect Close", reason, Alert.AlertType.ERROR);
        mainController.showStage("LoginForm",stageName);
    }

    @Override
    public void stageOnShowing(WindowEvent event) {
        backupHandler = mainController.getWebSocketHandler();
        mainController.setWebSocketHandler(this);
    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        friendListView.setCellFactory(p -> new FriendListItem());
        friendListView.setItems(FXCollections.observableArrayList(
                new FriendListItemModel("Hello", 123L, "qq.png"),
                new FriendListItemModel("Hello", 123L, "qq.png")
        ));
    }
}
