package Client.FriendListItem;

import javafx.beans.property.*;
import javafx.scene.image.Image;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;

public class FriendListItemModel {
    private final StringProperty nickName;
    private final LongProperty userID;
    private final LongProperty unread;
    private final ObjectProperty<Image> avatar;

    public FriendListItemModel(String nickName, Long userID, String avatar){
        this.nickName = new SimpleStringProperty(nickName);
        this.userID = new SimpleLongProperty(userID);
        this.avatar = new SimpleObjectProperty<>(new Image(avatar));
        this.unread = new SimpleLongProperty(0);
    }

    public FriendListItemModel(String nickName, Long userID, byte[] avatar){
        this.nickName = new SimpleStringProperty(nickName);
        this.userID = new SimpleLongProperty(userID);
        this.avatar = new SimpleObjectProperty<>(new Image(new ByteArrayInputStream(avatar)));
        this.unread = new SimpleLongProperty(0);
    }

    public FriendListItemModel(String nickName, Long userID, Image avatar){
        this.nickName = new SimpleStringProperty(nickName);
        this.userID = new SimpleLongProperty(userID);
        this.avatar = new SimpleObjectProperty<>(avatar);
        this.unread = new SimpleLongProperty(0);
    }
    public Image getAvatar() {
        return avatar.get();
    }

    public long getUserID() {
        return userID.get();
    }

    public long getUnread() {return unread.get();}

    public String getNickName() {
        return nickName.get();
    }

    public StringProperty nickNameProperty() {
        return nickName;
    }

    public LongProperty userIDProperty() {
        return userID;
    }

    public LongProperty unreadProperty() {
        return unread;
    }

    public ObjectProperty<Image> avatarProperty() {
        return avatar;
    }


}
