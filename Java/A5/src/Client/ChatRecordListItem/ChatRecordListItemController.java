package Client.ChatRecordListItem;

import javafx.beans.value.ChangeListener;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;

import java.text.SimpleDateFormat;
import java.util.Date;

public class ChatRecordListItemController {
    private final ChangeListener<String> NICKNAME_CHANGE_LISTENER = (property, oldValue, newValue) -> updateNickName(newValue);
    private final ChangeListener<String> CONTENT_CHANGE_LISTENER = (property, oldValue, newValue) -> updateNickName(newValue);
    //private final ChangeListener<Image> AVATAR_CHANGE_LISTENER = (property, oldValue, newValue) -> updateAvatar(newValue);
    private final ChangeListener<Number> ID_CHANGE_LISTENER = (property, oldValue, newValue) -> updateID(newValue);
    private final ChangeListener<Number> TIMESTAMP_CHANGE_LISTENER = (property, oldValue, newValue) -> updateTimestamp(newValue);
    @FXML
    private Label nickNameLabel;
    @FXML
    private Label idLabel;
    @FXML
    private Label timeLabel;
    @FXML
    private TextArea contentArea;

    private ChatRecordListItemModel model;

    public ChatRecordListItemModel getModel() {return model;}

    public void setModel(ChatRecordListItemModel model) {
        if(this.model != null)
            removeModelListener();
        this.model = model;
        setupModelListeners();
        updateView();
    }

    private void setupModelListeners() {
        model.nickNameProperty().addListener(NICKNAME_CHANGE_LISTENER);
        model.userIDProperty().addListener(ID_CHANGE_LISTENER);
        model.contentProperty().addListener(CONTENT_CHANGE_LISTENER);
        model.timestampProperty().addListener(TIMESTAMP_CHANGE_LISTENER);
    }

    private void removeModelListener() {
        model.contentProperty().removeListener(CONTENT_CHANGE_LISTENER);
        model.userIDProperty().removeListener(ID_CHANGE_LISTENER);
        model.timestampProperty().removeListener(TIMESTAMP_CHANGE_LISTENER);
        model.nickNameProperty().removeListener(NICKNAME_CHANGE_LISTENER);
    }

    private void updateView() {
        updateNickName();
        updateTimestamp();
        updateID();
        updateContent();
    }

    private void updateNickName() {
        updateNickName(model.getNickName());
    }

    private void updateNickName(String newValue) {
        nickNameLabel.setText(newValue);
    }

    private void updateTimestamp() {
        updateTimestamp(model.getTimestamp());
    }

    private void updateTimestamp(Number timestamp) {
        Date date = new Date(timestamp.longValue());
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        timeLabel.setText(dateFormat.format(date));
    }

    private void updateID() {
        updateID(model.getUserID());
    }

    private void updateID(Number userID) {
        idLabel.setText("("+userID.toString()+")");
    }

    private void updateContent() {
        updateContent(model.getContent());
    }

    private void updateContent(String content) {
        contentArea.setText(content);
    }
}

