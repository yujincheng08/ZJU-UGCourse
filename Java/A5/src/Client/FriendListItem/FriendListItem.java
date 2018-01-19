package Client.FriendListItem;

import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.ListCell;

import java.io.IOException;

public class FriendListItem extends ListCell<FriendListItemModel> {
    @Override
    protected void updateItem(FriendListItemModel model, boolean bool) {
        super.updateItem(model, bool);

        if (model != null) {

            FXMLLoader loader = new FXMLLoader(getClass().getResource("FriendListItem.fxml"));

            try {
                Node root = loader.load();
                FriendListItemController controller = loader.getController();
                controller.setModel(model);
                setGraphic(root);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
