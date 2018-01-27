package Client;

import Client.ChatRecordListItem.ChatRecordListItemModel;
import Client.FriendListItem.FriendListItem;
import Client.FriendListItem.FriendListItemModel;
import com.google.protobuf.InvalidProtocolBufferException;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.stage.WindowEvent;
import org.java_websocket.handshake.ServerHandshake;
import proto.MessageProto.Message;
import proto.FriendListMessageProto.FriendListMessage;
import proto.ChatMessageProto.ChatMessage;

import java.io.ByteArrayInputStream;
import java.net.URL;
import java.nio.ByteBuffer;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.ResourceBundle;

public class MainWindowController extends StagesController implements StageController, WebSocketHandler, Initializable {
    @FXML private ImageView avatar;
    @FXML private Label nickName;
    @FXML private Label id;
    @FXML private Button addButton;
    @FXML private TextField idField;
    @FXML private Tab friendTab;
    @FXML private Tab groupTab;
    @FXML
    private ListView<FriendListItemModel> groupListView;
    @FXML
    private ListView<FriendListItemModel> friendListView;
    private WebSocketHandler backupHandler;
    private ObservableList<FriendListItemModel> friendList = FXCollections.observableArrayList();
    private HashMap<Long, FriendListItemModel> friendModels = new HashMap<>();
    private FriendListItemModel myModel;
    private MainController controller;
    private String stageName;
    private enum Selection {
        FRIEND, GROUP
    }
    private static final String CHATDIALOGRES = "ChatDialog.fxml";

    private Selection currentSelection = Selection.FRIEND;

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
                    handleChatMessage(message.getChatMessageList());
                    break;
                case FriendListMessage:
                    if(message.getFriendListMesageCount() > 0)
                        handleFriendListMessage(message.getFriendListMesageList());
                    break;
                case ErrorMessage:
                    if(message.hasErrorMessage())
                        AlertHelper.show("",message.getErrorMessage(), Alert.AlertType.ERROR);
                    break;
            }
        } catch (InvalidProtocolBufferException e)
        {
            //Ignore it
        }

    }

    private void handleGroupMessage(ChatMessage chatMessage) {
        if(!chatMessage.hasContent() || !chatMessage.hasTimestamp())
            return;
        long groupID = chatMessage.getGroupID();
        String stageName = Selection.GROUP.toString() + groupID;
        if(isRegistered(stageName))
            updateStageMessage(stageName, chatMessage);
        //TODO
    }

    private void handlePrivateMessage(ChatMessage chatMessage) {
        if(!chatMessage.hasContent() || !chatMessage.hasTimestamp())
            return;
        long userID = chatMessage.getUserID();
        String stageName = Selection.FRIEND.toString() + userID;
        if(isRegistered(stageName))
            updateStageMessage(stageName, chatMessage);
        else if(friendModels.containsKey(userID)) {
            FriendListItemModel models = friendModels.get(userID);
            models.unreadProperty().setValue(models.getUnread() + 1);
        }
        try {
            controller.getDbController().storeChatRecord(myModel.getUserID(), chatMessage);
        }catch (SQLException e) {
            AlertHelper.show("Error", e.getMessage(), Alert.AlertType.ERROR);
        }
    }

    private void updateStageMessage(String stageName, ChatMessage chatMessage) {
        StageController stageController = getController(stageName);
        if(!(stageController instanceof ChatDialogController))
            return;
        ChatDialogController controller = (ChatDialogController) getController(stageName);
        long userID = chatMessage.getUserID();
        controller.addRecord(new ChatRecordListItemModel(chatMessage.getTimestamp(), friendModels.get(userID).getNickName(), userID, chatMessage.getContent()));
    }


    private void handleChatMessage(Iterable<ChatMessage> chatMessages) {
        for(ChatMessage chatMessage : chatMessages) {
            if (chatMessage.hasUserID())
                handlePrivateMessage(chatMessage);
            else if (chatMessage.hasGroupID())
                handleGroupMessage(chatMessage);
        }
    }

    private void addFriendItem(FriendListMessage message) {
        if(!message.hasAccount() || !message.hasNickname())
            return;
        long account = message.getAccount();
        String nickName = message.getNickname();
        FriendListItemModel model;
        if(message.hasAvatar())
            model = new FriendListItemModel(nickName, account, message.getAvatar().toByteArray());
        else
            model = new FriendListItemModel(nickName, account, "qq.png");
        friendList.add(model);
        friendModels.put(account, model);
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
                    updateSelfProfile(friendListMessage);
                    break;
                case REQUIRE:
                    //invalid and ignore
                    break;
            }
        }
    }

    private void updateSelfProfile(FriendListMessage friendListMessage) {
        if(!friendListMessage.hasAccount() || !friendListMessage.hasNickname())
            return;
        long account = friendListMessage.getAccount();
        String nickName = friendListMessage.getNickname();
        id.setText(String.valueOf(account));
        this.nickName.setText(nickName);
        if(friendListMessage.hasAvatar())
            avatar.setImage(new Image(new ByteArrayInputStream(friendListMessage.getAvatar().toByteArray())));
        else
            avatar.setImage(new Image("qq.png"));
        myModel = new FriendListItemModel(nickName, account, avatar.getImage());
        friendModels.put(account, myModel);
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
        friendTab.setOnSelectionChanged(e->{
            addButton.setText("Add friend");
            currentSelection = Selection.FRIEND;
        });
        groupTab.setOnSelectionChanged(e->{
            addButton.setText("Join Group");
            currentSelection = Selection.GROUP;
        });
        addButton.setOnAction(e->add());
        friendListView.setOnMouseClicked(e->{
            if(e.getButton().equals(MouseButton.PRIMARY) && e.getClickCount() == 2)
                openChatDialog(friendListView.getSelectionModel());
        });
    }

    private void sendMessage(long userID, long timestamp, String message) {
        ChatMessage.Builder builder = ChatMessage.newBuilder();
        builder.setTimestamp(timestamp);
        builder.setUserID(userID);
        builder.setContent(message);
        ChatMessage chatMessage = builder.build();
        controller.webSocketSend(chatMessage);
        try {
            this.controller.getDbController().storeSendRecord(myModel.getUserID(), chatMessage);
        } catch (SQLException e) {
            AlertHelper.show("Error", e.getMessage(), Alert.AlertType.ERROR);
        }
    }

    private void openChatDialog(MultipleSelectionModel<FriendListItemModel> selectionModel) {
        FriendListItemModel model = selectionModel.getSelectedItem();
        long userID = model.getUserID();
        String stageName = currentSelection.toString() + userID;
        if(!isRegistered(stageName))
            registerStage(stageName, CHATDIALOGRES);
        StageController controller = getController(stageName);
        if(controller instanceof ChatDialogController) {
            ChatDialogController chatDialogController = (ChatDialogController) controller;
            chatDialogController.setModel(model);
            chatDialogController.setOnSendMessage(message->{
                long timestamp = new Date().getTime();
                sendMessage(userID, timestamp, message);
                chatDialogController.addRecord(new ChatRecordListItemModel(timestamp, nickName.getText(), myModel.getUserID(), message));
            });
            chatDialogController.setRequireMessage(((start, end) -> requirePage(chatDialogController, userID, start, end)));
            friendModels.get(userID).unreadProperty().set(0);
        }
        showStage(stageName);
    }

    @Override
    public void stageClosed(String name){
        deregisterStage(name);
    }

    private void add() {
        String ID = idField.getText();
        try {
            long account = Long.parseLong(ID);
            FriendListMessage.Builder builder = FriendListMessage.newBuilder();
            switch (currentSelection) {
                case FRIEND:
                    builder.setType(FriendListMessage.Type.ADDFRIEND);
                    break;
                case GROUP:
                    builder.setType(FriendListMessage.Type.ADDGROUP);
                    break;
            }
            builder.setAccount(account);
            controller.webSocketSend(builder.build());
        }catch(ArithmeticException e) {
            AlertHelper.show(addButton.getText(), "Error id number.", Alert.AlertType.ERROR);
        }
    }

    void requirePage(ChatDialogController controller, long userID, int start, int end) {
        try {
            ArrayList<ChatRecordListItemModel> page = new ArrayList<>(end-start);
            for(ChatMessage message : this.controller.getDbController().getChartRecord(userID, end-start, start)){
                page.add(new ChatRecordListItemModel(message.getTimestamp(), friendModels.get(message.getUserID()).getNickName(), message.getUserID(), message.getContent()));
            }
            controller.setPage(page, start);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
