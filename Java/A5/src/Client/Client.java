package Client;
import javafx.application.Application;
import javafx.stage.Stage;

public class Client extends Application {

    private MainController mainController = new MainController();
    private static final String mainWindowName = "MainWindow";
    private static final String mainWindowResource = "MainWindow.fxml";

    private static final String loginFormName = "LoginForm";
    private static final String loginFormResource = "LoginForm.fxml";

    @Override
    public void start(Stage primaryStage)
    {
        mainController.setPrimaryStage(primaryStage);

        mainController.loadStage(mainWindowName, mainWindowResource);
        mainController.loadStage(loginFormName, loginFormResource);

        mainController.showStage(loginFormName);
    }

    public static void main(String[] args)
    {
        launch(args);
    }
}
