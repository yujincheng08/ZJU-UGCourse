package Client.ChatRecordListItem;

import javafx.beans.property.LongProperty;
import javafx.beans.property.SimpleLongProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class ChatRecordListItemModel {
    private final LongProperty timestamp;
    private final StringProperty nickName;
    private final LongProperty userID;
    //private final ObjectProperty<Image> avatar;
    private final StringProperty content;

    public ChatRecordListItemModel(long timestamp, String nickName, long userID, String content) {
        this.nickName = new SimpleStringProperty(nickName);
        this.userID = new SimpleLongProperty(userID);
        this.timestamp = new SimpleLongProperty(timestamp);
        this.content = new SimpleStringProperty(content);
    }

    public long getUserID() {return userID.get();}

    public String getNickName() {return nickName.get();}

    public long getTimestamp() {return timestamp.get();}

    public String getContent() {return content.get();}

    public LongProperty timestampProperty() {
        return timestamp;
    }

    public LongProperty userIDProperty() {
        return userID;
    }

    public StringProperty contentProperty() {
        return content;
    }

    public StringProperty nickNameProperty() {
        return nickName;
    }
}
