package Client.FriendListItem;

import javafx.beans.value.ChangeListener;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

public class FriendListItemController {
    private final ChangeListener<String> NICKNAME_CHANGE_LISTENER = (property, oldValue, newValue) -> updateNickName(newValue);
    private final ChangeListener<Image> AVATOR_CHANGE_LISTENER = (property, oldValue, newValue) -> updateAvatar(newValue);
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
        model.avatarProperty().removeListener(AVATOR_CHANGE_LISTENER);
    }

    private void setupModelListeners() {
        model.nickNameProperty().addListener(NICKNAME_CHANGE_LISTENER);
        model.avatarProperty().addListener(AVATOR_CHANGE_LISTENER);
    }

    private void updateView() {
        updateNickName();
        updateAvatar();
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

}
