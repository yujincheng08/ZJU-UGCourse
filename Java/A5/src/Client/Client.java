package Client;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.web.HTMLEditor;
import javafx.stage.Stage;

public class Client extends Application {

    @Override
    public void start(Stage stage) {

        stage.setTitle("HTMLEditor Sample");
        stage.setWidth(500);
        stage.setHeight(300);

        final HTMLEditor htmlEditor = new HTMLEditor();
        htmlEditor.setPrefHeight(245);
        Scene scene = new Scene(htmlEditor);
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
