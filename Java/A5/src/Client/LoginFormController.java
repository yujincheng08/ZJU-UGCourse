package Client;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;

import java.awt.event.ActionEvent;
import java.net.URL;
import java.util.ResourceBundle;

public class LoginFormController implements ControlledStage
{
    public CheckBox savePassword;
    public Button loginButton;
    public TextField account;
    public PasswordField password;
    private MainController mainController;
    private String stageName;

    @Override
    public void setMainController(MainController mainController, String currentStageName)
    {
        this.mainController = mainController;
        this.stageName = currentStageName;
    }
    @FXML protected void login()
    {
        System.out.println(account.getText().trim());
        System.out.println(password.getText());
        mainController.showStage("MainWindow", stageName);
    }

}
