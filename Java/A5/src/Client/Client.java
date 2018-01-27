package Client;

import javafx.application.Application;
import javafx.stage.Stage;

public class Client extends Application {
    static int port;
    private MainController mainController = new MainController(port);
    private static final String mainWindowName = "MainWindow";
    private static final String mainWindowResource = "MainWindow.fxml";

    private static final String loginFormName = "LoginForm";
    private static final String loginFormResource = "LoginForm.fxml";

    @Override
    public void start(Stage primaryStage) {
        mainController.setPrimaryStage(primaryStage);

        mainController.registerStage(mainWindowName, mainWindowResource);
        mainController.registerStage(loginFormName, loginFormResource);

        mainController.showStage(loginFormName);
    }

    public static void main(String[] args) {
        try {
            port  = Integer.parseInt(args[0]);
        }catch(ArithmeticException e) {
            port = 9000;
        }
        launch(args);
    }
}
