package Client;

import Client.FriendListItem.FriendListItem;
import Client.FriendListItem.FriendListItemModel;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ListView;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URL;
import java.util.ResourceBundle;

public class MainWindowController implements StageController, WebSocketHandler, Initializable {
    @FXML
    private ListView groupListView;
    @FXML
    private ListView<FriendListItemModel> friendListView;
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

    @Override
    public void onClose(int code, String reason, boolean remote) {

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
