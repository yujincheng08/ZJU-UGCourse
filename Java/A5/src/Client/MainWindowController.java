package Client;

import Client.FriendListItem.FriendListItem;
import Client.FriendListItem.FriendListItemModel;
import com.google.protobuf.InvalidProtocolBufferException;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.ListView;
import javafx.stage.WindowEvent;
import org.java_websocket.handshake.ServerHandshake;
import proto.MessageProto.Message;
import proto.FriendListMessageProto.FriendListMessage;

import java.net.URL;
import java.nio.ByteBuffer;
import java.util.ResourceBundle;

public class MainWindowController extends StagesController implements StageController, WebSocketHandler, Initializable {
    @FXML
    private ListView groupListView;
    @FXML
    private ListView<FriendListItemModel> friendListView;
    private WebSocketHandler backupHandler;
    private ObservableList<FriendListItemModel> friendList = FXCollections.observableArrayList();
    private MainController controller;
    private String stageName;

    @Override
    public void setStagesController(StagesController controller, String currentStageName) {
        if(controller instanceof MainController)
            this.controller = (MainController)controller;
        this.stageName = currentStageName;
    }

    @Override
    public void onError(Exception exception) {

    }

    @Override
    public void onMessage(ByteBuffer buffer) {
        try
        {
            Message message = Message.parseFrom(buffer);
            if(!message.hasType())
                return;
            switch(message.getType()){
                case ChatMessage:
                    break;
                case FriendListMessage:
                    if(message.getFriendListMesageCount() > 0)
                    handleFriendListMessage(message.getFriendListMesageList());
                    break;
            }
        } catch (InvalidProtocolBufferException e)
        {
            //Ignore it
        }

    }

    private void addFriendItem(FriendListMessage message) {
        if(!message.hasAccount() || !message.hasNickname())
            return;
        long account = message.getAccount();
        String nickName = message.getNickname();
        if(message.hasAvatar())
            friendList.add(new FriendListItemModel(nickName, account, message.getAvatar().toByteArray()));
        else
            friendList.add(new FriendListItemModel(nickName, account, "assets/qq.png"));
    }

    private void handleFriendListMessage(Iterable<FriendListMessage> friendListMessages)
    {
        for(FriendListMessage friendListMessage : friendListMessages){
            if(!friendListMessage.hasType())
                continue;
            switch(friendListMessage.getType()) {
                case FRIEND:
                    addFriendItem(friendListMessage);
                    break;
                case GROUP:
                    break;
                case FRIENDSEARCH:
                    break;
                case GROUPSEARCH:
                    break;
                case SELF:
                    break;
                case REQUIRE:
                    //invalid and ignore
                    break;
            }
        }
    }

    public void onClose(int code, String reason, boolean remote) {
        controller.setWebSocketHandler(backupHandler);
        AlertHelper.show("Connect Close", reason, Alert.AlertType.ERROR);
        controller.showStage("LoginForm",stageName);
        friendList.clear();

    }

    @Override
    public void stageOnShowing(WindowEvent event) {
        backupHandler = controller.getWebSocketHandler();
        controller.setWebSocketHandler(this);
        //controller.webSocketSend();

        //Require for friendList and GroupList
        FriendListMessage.Builder builder = FriendListMessage.newBuilder();
        builder.setType(FriendListMessage.Type.REQUIRE);
        controller.webSocketSend(builder.build());
    }

    @Override
    public void stageOnHiding(WindowEvent event){
        controller.setWebSocketHandler(backupHandler);
    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        friendListView.setCellFactory(p -> new FriendListItem());
        friendListView.setItems(friendList);
    }
}
