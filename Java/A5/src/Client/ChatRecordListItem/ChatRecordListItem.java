package Client.ChatRecordListItem;

import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.ListCell;

import java.io.IOException;

public class ChatRecordListItem extends ListCell<ChatRecordListItemModel> {
    @Override
    protected void updateItem(ChatRecordListItemModel model, boolean bool) {
        super.updateItem(model, bool);

        if(model != null) {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("ChatRecordListItem.fxml"));

            try {
                Node root = loader.load();
                ChatRecordListItemController controller = loader.getController();
                controller.setModel(model);
                setGraphic(root);
            }catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
