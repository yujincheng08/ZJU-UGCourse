package Client;

import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;
import javafx.stage.WindowEvent;
import org.java_websocket.handshake.ServerHandshake;
import proto.LoginMessageProto.LoginMessage;

import java.net.URL;
import java.nio.ByteBuffer;
import java.util.ResourceBundle;

public class RegisterFormController implements StageController, WebSocketHandler, Initializable{

    public TextField nickName;
    public TextField mail;
    public PasswordField password;
    public PasswordField repeatPassword;
    public Button registerButton;
    public Button cancelButton;
    public AnchorPane root;

    private MainController mainController;
    private WebSocketHandler backupHandler;

    @Override
    public void setMainController(MainController mainController, String currentStageName) {
        this.mainController = mainController;
    }

    @Override
    public void stageOnShown(WindowEvent event) {
        backupHandler = mainController.getWebSocketHandler();
        mainController.setWebSocketHandler(this);
    }

    @Override
    public void stageOnHidden(WindowEvent event) {
        mainController.setWebSocketHandler(backupHandler);
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        cancelButton.setOnAction(e->root.getScene().getWindow().hide());
        registerButton.setOnAction(e->register());
    }

    private void register() {
        if(password.getText().isEmpty() || nickName.getText().isEmpty() || repeatPassword.getText().isEmpty() || mail.getText().isEmpty())
            AlertHelper.show("Input Error", "Please input valid value.", Alert.AlertType.ERROR);
        else if(!password.getText().equals(repeatPassword.getText()))
            AlertHelper.show("Input Error", "Password does not match.", Alert.AlertType.ERROR);
        else if(!mail.getText().matches(".*@.*"))
            AlertHelper.show("Input Error", "Mail address format is not valid.", Alert.AlertType.ERROR);
        else {
            LoginMessage.Builder builder = LoginMessage.newBuilder();
            builder.setType(LoginMessage.Type.REGISTER);
            builder.setEmail(mail.getText());
            builder.setPassword(password.getText());
            builder.setNickName(nickName.getText());
            mainController.webSocketSend(builder.build());
        }
    }

    @Override
    public void onError(Exception exception) {

    }

    @Override
    public void onMessage(ByteBuffer message) {

    }

    @Override
    public void onClose(int code, String reason, boolean remote) {

    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }
}
