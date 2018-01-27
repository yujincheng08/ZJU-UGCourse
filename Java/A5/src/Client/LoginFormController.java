package Client;

import com.google.protobuf.InvalidProtocolBufferException;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.AnchorPane;
import javafx.stage.WindowEvent;
import org.java_websocket.handshake.ServerHandshake;
import proto.LoginMessageProto.LoginMessage;
import proto.MessageProto.Message;
import sun.applet.Main;

import java.net.URL;
import java.nio.ByteBuffer;
import java.sql.SQLException;
import java.util.ResourceBundle;

public class LoginFormController implements StageController, WebSocketHandler, Initializable {
    private static final String registerFormName = "RegisterForm";
    private static final String registerFormResource = "RegisterForm.fxml";

    @FXML
    private CheckBox savePassword;
    @FXML
    private Button loginButton;
    @FXML
    private TextField account;
    @FXML
    private PasswordField password;
    @FXML
    private CheckBox autoLogin;
    @FXML
    private AnchorPane root;
    @FXML
    private Hyperlink register;
    private MainController mainController;
    private String stageName;
    private WebSocketHandler backupHandler;

    @Override
    public void setStagesController(StagesController controller, String currentStageName) {
        if(controller instanceof MainController)
            this.mainController = (MainController)controller;
        this.stageName = currentStageName;

    }

    @FXML
    protected void login() {
        String prompt = null;
        String password = this.password.getText();
        long account = 0;
        try {
            account = Long.parseLong(this.account.getText().trim());
        } catch (NumberFormatException e) {
            prompt = "Account can only be numbers.";
        }
        if (password.length() == 0)
            prompt = "Password cannot be empty.";
        if (prompt != null)
            AlertHelper.show("Input Invalid", prompt, Alert.AlertType.ERROR);
        else {
            LoginMessage.Builder builder = LoginMessage.newBuilder();
            builder.setType(LoginMessage.Type.LOGIN);
            builder.setAccount(account);
            builder.setPassword(MD5Encoder.encode(password));
            mainController.webSocketSend(builder.build());
        }
    }

    @Override
    public void stageOnShowing(WindowEvent event) {
        if(mainController != null) {
            backupHandler = mainController.getWebSocketHandler();
            mainController.setWebSocketHandler(this);
        }
    }

    @Override
    public void onError(Exception exception) {
        AlertHelper.show("Connection Error", exception.getMessage(), Alert.AlertType.ERROR);
    }

    @Override
    public void onMessage(ByteBuffer bytes) {
        try {
            Message message = Message.parseFrom(bytes.array());
            if (message.hasType() && message.getType() == Message.Type.LoginMessage
                    && message.hasLoginMessage()) {
                LoginMessage loginMessage = message.getLoginMessage();
                //noinspection StatementWithEmptyBody
                if (!loginMessage.hasType() || loginMessage.getType() != LoginMessage.Type.LOGIN) {
                    //do nothing
                } else if (loginMessage.hasStatus() && loginMessage.getStatus() == LoginMessage.Status.SUCCESS) {
                    mainController.setDbController(loginMessage.getAccount());
                    mainController.showStage("MainWindow", stageName);
                }
                else {
                    String prompt;
                    if (loginMessage.hasPrompt())
                        prompt = loginMessage.getPrompt();
                    else
                        prompt = "Unknown error. Please retry.";
                    AlertHelper.show("Login failed", prompt, Alert.AlertType.ERROR);
                }
            }
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            AlertHelper.show("Login failed", e.getMessage(), Alert.AlertType.ERROR);
        }

    }

    @Override
    public void onClose(int code, String reason, boolean remote) {
        if(mainController != null) mainController.setWebSocketHandler(backupHandler);
    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }

    private void showRegisterDialog() {
        mainController.showDialog(registerFormName, registerFormResource, stageName);
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        autoLogin.selectedProperty().addListener((ov, o, n) -> {
            if (n)
                savePassword.setSelected(true);
        });
        savePassword.selectedProperty().addListener((ov, o, n) -> {
            if (!n)
                autoLogin.setSelected(false);
        });
        root.addEventHandler(KeyEvent.KEY_PRESSED, ev -> {
            if (ev.getCode() == KeyCode.ENTER) {
                loginButton.fire();
                ev.consume();
            }
        });
        loginButton.setOnAction(e -> login());
        loginButton.setDefaultButton(true);
        register.setOnAction(e -> showRegisterDialog());
    }
}
