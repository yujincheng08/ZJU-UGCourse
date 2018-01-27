package Client;

import Client.ChatRecordListItem.ChatRecordListItem;
import Client.ChatRecordListItem.ChatRecordListItemModel;
import Client.FriendListItem.FriendListItemModel;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextArea;
import javafx.scene.image.ImageView;
import javafx.scene.layout.AnchorPane;
import javafx.stage.WindowEvent;

import java.net.URL;
import java.util.Collection;
import java.util.Comparator;
import java.util.ResourceBundle;

public class ChatDialogController implements StageController, Initializable{
    @FXML private ImageView avatar;
    @FXML private Label nickName;
    @FXML private Label accountID;
    @FXML private ListView<ChatRecordListItemModel> chatRecord;
    private ObservableList<ChatRecordListItemModel> records = FXCollections.observableArrayList();
    @FXML private TextArea textField;
    @FXML private Button sendButton;
    @FXML private Button prePage;
    @FXML private Button closeButton;
    @FXML private Button nextPage;
    @FXML private ListView groupMembers;
    private StagesController controller;
    private String name;
    private int bottom = 0;
    private MessageRequire requirer;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        closeButton.setOnAction(e->controller.getStage(name).close());
        prePage.setOnAction(e->prePage());
        nextPage.setOnAction(e->nextPage());
        chatRecord.setCellFactory(p -> new ChatRecordListItem());
        chatRecord.setItems(records);
    }

    interface MessageListener {
        void onMessageSend(String message);
    }

    interface MessageRequire {
        void require(int start, int end);
    }


    @Override
    public void setStagesController(StagesController stagesController, String currentStageName) {
        controller = stagesController;
        name = currentStageName;
    }

    @Override
    public void stageOnShowing(WindowEvent event) {
        if(requirer!=null) {
            requirer.require(bottom, bottom+10);
        }
    }

    @Override
    public void stageOnShown(WindowEvent event) {

    }

    @Override
    public void stageOnHidden(WindowEvent event) {
    }

    @Override
    public void stageOnHiding(WindowEvent event) {

    }

    public void nextPage() {
        int start = Math.max(bottom-10, 0);
        requirer.require(start, bottom);
    }

    public void prePage() {
        requirer.require(bottom+10, bottom+20);
    }

    public void setModel(FriendListItemModel model){
        accountID.setText(String.valueOf(model.getUserID()));
        avatar.setImage(model.getAvatar());
        nickName.setText(model.getNickName());
    }

    public void addRecord(ChatRecordListItemModel model) {
        if(bottom > 0) {
            bottom++;
            return;
        }
        records.add(model);
        records.sort(Comparator.comparingInt(x -> (int) x.getTimestamp()));
        if(records.size() > 10)
            records.remove(0, records.size()-11);
    }

    public void setPage(Collection<? extends ChatRecordListItemModel> model, int bottom) {
        records.clear();
        records.addAll(model);
        records.sort(Comparator.comparingInt(x -> (int) x.getTimestamp()));
        this.bottom = bottom;
        chatRecord.scrollTo(records.size()-1);
        if(model.size() < 10)
            prePage.setDisable(true);
        else
            prePage.setDisable(false);
        if(bottom == 0)
            nextPage.setDisable(true);
        else
            nextPage.setDisable(false);
    }

    public void addRecord(Iterable<? extends ChatRecordListItemModel> models) {
        for(ChatRecordListItemModel model : models)
            addRecord(model);
    }

    public void setOnSendMessage(MessageListener listener){
        sendButton.setOnAction(e->{
            if(!textField.getText().isEmpty())
                listener.onMessageSend(textField.getText());
            textField.clear();
        });
    }

    public void setRequireMessage(MessageRequire requirer) {
        this.requirer = requirer;
    }
}
