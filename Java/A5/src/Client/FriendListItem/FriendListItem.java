package Client;

import javafx.fxml.FXMLLoader;
import javafx.fxml.JavaFXBuilderFactory;
import javafx.scene.Node;
import javafx.scene.control.ListCell;

import java.io.IOException;
import java.net.URL;

public class FriendListItem extends ListCell<FriendListItemModel>{
    @Override
    protected void updateItem(FriendListItemModel model, boolean bln)
    {
        super.updateItem(model, bln);

        if(model != null)
        {
            URL location = FriendListItemController.class.getResource("ChoiceView.fxml");

            FXMLLoader fxmlLoader = new FXMLLoader();
            fxmlLoader.setLocation(location);
            fxmlLoader.setBuilderFactory(new JavaFXBuilderFactory());

            try
            {
                Node root = (Node)fxmlLoader.load(location.openStream());
                FriendListItemController controller = (FriendListItemController) fxmlLoader.getController();
                controller.setModel(model);
                setGraphic(root);
            }
            catch(IOException ioe)
            {
                throw new IllegalStateException(ioe);
            }
        }
    }
}
