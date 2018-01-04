package Client;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

import java.util.Date;

public class ChatRecord {
    private ObjectProperty<Date> timestamp;
    private StringProperty content;


    public ChatRecord(long timestamp, String content)
    {
        this.timestamp = new SimpleObjectProperty<>(new Date(timestamp));
        this.content = new SimpleStringProperty(content);
    }

    public long getTimestamp() {
        return timestamp.get().getTime();
    }

    public String getContent() {
        return content.get();
    }

    public ObjectProperty<Date> timestampProperty()
    {
        return timestamp;
    }

    public StringProperty contentProperty()
    {
        return content;
    }

    public void setContent(String content) {
        this.content.setValue(content);
    }

    public void setTimestamp(long timestamp) {
        this.timestamp.setValue(new Date(timestamp));
    }

    @Override
    public String toString() {
        return "timestamp: " + getTimestamp() + "\tcontent: " + getContent();
    }
}
