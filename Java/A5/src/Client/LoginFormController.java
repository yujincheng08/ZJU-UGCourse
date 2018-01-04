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

import java.math.BigInteger;
import java.net.URL;
import java.nio.ByteBuffer;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ResourceBundle;

public class LoginFormController implements StageController, WebSocketHandler, Initializable
{
    private static final String registerFormName = "RegisterName";
    private static final String registerFormResource = "RegisterForm.fxml";

    public CheckBox savePassword;
    public Button loginButton;
    public TextField account;
    public PasswordField password;
    public CheckBox autoLogin;
    public AnchorPane root;
    public Hyperlink register;
    private MainController mainController;
    private String stageName;
    private WebSocketHandler backupHandler;

    private String MD5Encoder(String str)
    {
        try {
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            byte[] digest = md5.digest(str.getBytes());
            BigInteger bigInteger = new BigInteger(1, digest);
            StringBuilder hash = new StringBuilder(bigInteger.toString(16));
            while(hash.length()<32)
                hash.insert(0, "0");
            return hash.toString();
        }catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return null;
    }



    @Override
    public void setMainController(MainController mainController, String currentStageName)
    {
        this.mainController = mainController;
        this.stageName = currentStageName;

    }

    @FXML protected void login()
    {
        String account = this.account.getText().trim();
        String password = this.password.getText();
        if(account.length()==0 || password.length() == 0)
            AlertHelper.show("Input Error", "Please input valid account and password", Alert.AlertType.ERROR);
        else
        {
            LoginMessage.Builder builder = LoginMessage.newBuilder();
            builder.setType(LoginMessage.Type.LOGIN);
            builder.setAccount(account);
            builder.setPassword(MD5Encoder(password));
            mainController.webSocketSend(builder.build());
        }
    }

    @Override
    public void stageOnShowing(WindowEvent event) {
        backupHandler = mainController.getWebSocketHandler();
        mainController.setWebSocketHandler(this);
    }

    @Override
    public void onError(Exception exception) {
        AlertHelper.show("Connection Error", exception.getMessage(), Alert.AlertType.ERROR);
    }

    @Override
    public void onMessage(ByteBuffer bytes) {
        try {
            Message message = Message.parseFrom(bytes.array());
            if(message.hasType() && message.getType() == Message.Type.LoginMessage
                    && message.hasLoginMessage()) {
                LoginMessage loginMessage = message.getLoginMessage();
                if (loginMessage.hasStatus() && loginMessage.getStatus() == LoginMessage.Status.SUCCESS)
                    mainController.showStage("MainWindow", stageName);
                else {
                    String prompt;
                    if (loginMessage.hasPrompt())
                        prompt = loginMessage.getPrompt();
                    else
                        prompt = "Unknown error. Please retry.";
                    AlertHelper.show("Login failed", prompt, Alert.AlertType.ERROR);
                }
            }
        }catch(InvalidProtocolBufferException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void onClose(int code, String reason, boolean remote) {
        mainController.setWebSocketHandler(backupHandler);
    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }

    private void showRegisterDialog()
    {
        mainController.showDialog(registerFormName, registerFormResource, stageName);
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
         autoLogin.selectedProperty().addListener((ov, o, n) -> {
            if(n)
                savePassword.setSelected(true);
        });
        savePassword.selectedProperty().addListener((ov, o, n) -> {
            if(!n)
                autoLogin.setSelected(false);
        });
        root.addEventHandler(KeyEvent.KEY_PRESSED, ev -> {
            if (ev.getCode() == KeyCode.ENTER) {
                loginButton.fire();
                ev.consume();
            }
        });
        loginButton.setOnAction(e-> login());
        loginButton.setDefaultButton(true);
        register.setOnAction(e-> showRegisterDialog());
    }
}
