package Client;

import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

import java.util.HashMap;

public class MainController
{
    private static final String PRIMARY_STAGE_NAME = "PrimaryStage";
    private HashMap<String, Stage> stages = new HashMap<>();

    public void addStage(String name, Stage stage)
    {
        stages.put(name, stage);
    }

    public Stage getStage(String name)
    {
        return stages.get(name);
    }

    public void setPrimaryStage(Stage primaryStage)
    {
        if(!stages.containsKey(PRIMARY_STAGE_NAME))
            addStage(PRIMARY_STAGE_NAME, primaryStage);
        else
            throw new RuntimeException("Primary stage already exists");
    }

    public boolean loadStage(String name, String resource, StageStyle... styles)
    {
        try{
            FXMLLoader loader = new FXMLLoader(getClass().getResource(resource));
            Pane tmpPane = loader.load();

            Scene tmpScene = new Scene(tmpPane);
            Stage tmpStage = new Stage();
            tmpStage.setScene(tmpScene);
            for (StageStyle style : styles)
                tmpStage.initStyle(style);

            tmpStage.setTitle(name);

            this.addStage(name, tmpStage);

            ControlledStage controlledStage = loader.getController();
            controlledStage.setMainController(this, name);

            return true;
        }catch(Exception e){
            e.printStackTrace();
            return false;
        }
    }

    public void showStage(String name)
    {
        this.getStage(name).show();
    }

    public void showStage(String showName, String closeName)
    {
        getStage(closeName).close();
        showStage(showName);
    }

    public void unloadStage(String name)
    {
        Stage tmpStage = stages.remove(name);
        if(tmpStage == null)
            throw new RuntimeException("Stage " + name + " does not exists");
        else
            tmpStage.close();
    }
}
