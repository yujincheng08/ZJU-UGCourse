package Client;

public class MainWindowController implements ControlledStage
{
    private MainController mainController;
    private String stageName;
    @Override
    public void setMainController(MainController mainController, String currentStageName)
    {
        this.mainController = mainController;
        this.stageName = currentStageName;
    }
}
