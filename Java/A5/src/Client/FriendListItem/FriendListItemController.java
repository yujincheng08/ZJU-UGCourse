package Client.FriendListItem;

import javafx.beans.value.ChangeListener;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

public class FriendListItemController {
    private final ChangeListener<String> NICKNAME_CHANGE_LISTENER = (property, oldValue, newValue) -> updateNickName(newValue);
    private final ChangeListener<Image> AVATAR_CHANGE_LISTENER = (property, oldValue, newValue) -> updateAvatar(newValue);
    private final ChangeListener<Number> ID_CHANGE_LISTENER = (property, oldValue, newValue) -> updateID(newValue);
    private final ChangeListener<Number> UNREAD_CHANGE_LISTENER = (property, oldValue, newValue) -> updateUnread(newValue);


    @FXML
    private Label unread;
    @FXML
    private Label accountID;
    @FXML
    private ImageView avatar;
    @FXML
    private Label nickName;


    private FriendListItemModel model;

    public FriendListItemModel getModel() {
        return model;
    }

    public void setModel(FriendListItemModel model) {
        if (this.model != null)
            removeModelListeners();
        this.model = model;
        setupModelListeners();
        updateView();
    }

    private void removeModelListeners() {
        model.nickNameProperty().removeListener(NICKNAME_CHANGE_LISTENER);
        model.avatarProperty().removeListener(AVATAR_CHANGE_LISTENER);
        model.userIDProperty().removeListener(ID_CHANGE_LISTENER);
        model.unreadProperty().removeListener(UNREAD_CHANGE_LISTENER);
    }

    private void setupModelListeners() {
        model.nickNameProperty().addListener(NICKNAME_CHANGE_LISTENER);
        model.avatarProperty().addListener(AVATAR_CHANGE_LISTENER);
        model.userIDProperty().addListener(ID_CHANGE_LISTENER);
        model.unreadProperty().addListener(UNREAD_CHANGE_LISTENER);
    }

    private void updateView() {
        updateNickName();
        updateAvatar();
        updateID();
        updateUnread();
    }

    private void updateNickName() {
        updateNickName(model.getNickName());
    }

    private void updateNickName(String newValue) {
        nickName.setText(newValue);
    }

    private void updateAvatar() {
        updateAvatar(model.getAvatar());
    }

    private void updateAvatar(Image newValue) {
        avatar.setImage(newValue);
    }

    private void updateID(Number newValue) {accountID.setText(newValue.toString());}

    private void updateID() {updateID(model.getUserID());}

    private void updateUnread() {
        updateUnread(model.getUnread());
    }

    private void updateUnread(Number newValue) {
        if(newValue.equals(0))
            unread.setVisible(false);
        else {
            unread.setVisible(true);
            unread.setText(newValue.toString());
        }
    }

}
