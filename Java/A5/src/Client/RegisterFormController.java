package Client;

import com.google.protobuf.InvalidProtocolBufferException;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;
import javafx.stage.WindowEvent;
import org.java_websocket.handshake.ServerHandshake;
import proto.LoginMessageProto.LoginMessage;
import proto.MessageProto;

import java.net.URL;
import java.nio.ByteBuffer;
import java.util.ResourceBundle;

public class RegisterFormController implements StageController, WebSocketHandler, Initializable{

    @FXML private TextField nickName;
    @FXML private TextField mail;
    @FXML private PasswordField password;
    @FXML private PasswordField repeatPassword;
    @FXML private Button registerButton;
    @FXML private Button cancelButton;
    @FXML private AnchorPane root;

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
            builder.setPassword(MD5Encoder.encode(password.getText()));
            builder.setNickName(nickName.getText());
            mainController.webSocketSend(builder.build());
        }
    }

    @Override
    public void onError(Exception exception) {

    }

    @Override
    public void onMessage(ByteBuffer bytes) {
        try {
            MessageProto.Message message = MessageProto.Message.parseFrom(bytes.array());
            if(message.hasType() && message.getType() == MessageProto.Message.Type.LoginMessage
                    && message.hasLoginMessage()) {
                LoginMessage loginMessage = message.getLoginMessage();
                //noinspection StatementWithEmptyBody
                if(!loginMessage.hasType() || loginMessage.getType() != LoginMessage.Type.REGISTER){
                }
                else if (loginMessage.hasStatus() && loginMessage.hasAccount() && loginMessage.getStatus() == LoginMessage.Status.SUCCESS)
                    AlertHelper.show("Register success",
                            "Your id is " + loginMessage.getAccount() + ". Please well remember it.", Alert.AlertType.INFORMATION);
                else {
                    String prompt;
                    if (loginMessage.hasPrompt())
                        prompt = loginMessage.getPrompt();
                    else
                        prompt = "Unknown error. Please retry.";
                    AlertHelper.show("Login failed", prompt, Alert.AlertType.ERROR);
                    cancelButton.fire();
                }
            }
        }catch(InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onClose(int code, String reason, boolean remote) {

    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }
}
