package Client;

import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import javafx.stage.Window;

import java.io.IOException;
import java.util.HashMap;

class StagesController
{
    protected static final String PRIMARY_STAGE_NAME = "PrimaryStage";
    protected HashMap<String, Stage> stages = new HashMap<>();
    protected int shownStageCount = 0;

    protected void stageClosed(String name){}
    protected void stageShown(String name){}

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

    public Stage loadStage(String name, String resource, StageStyle... styles)
            throws IOException
    {
        FXMLLoader loader = new FXMLLoader(getClass().getResource(resource));
        Pane tmpPane = loader.load();

        Stage tmpStage = new Stage();
        Scene tmpScene = new Scene(tmpPane);
        tmpStage.setScene(tmpScene);
        StageController stageController = loader.getController();
        stageController.setStagesController(this, name);

        tmpStage.setOnShown(e -> {
            stageController.stageOnShown(e);
            ++shownStageCount;
            stageShown(name);
        });
        tmpStage.setOnShowing(stageController::stageOnShowing);
        tmpStage.setOnHiding(stageController::stageOnHiding);
        tmpStage.setOnHidden(e -> {
            stageController.stageOnHidden(e);
            --shownStageCount;
            stageClosed(name);
        });
        for (StageStyle style : styles)
            tmpStage.initStyle(style);
        tmpStage.setTitle(name);
        return tmpStage;
    }

    public boolean registerStage(String name, String resource, StageStyle... styles)
    {
        if(getStage(name) != null)
            return false;
        try{

            Stage tmpStage = loadStage(name, resource, styles);
            this.addStage(name, tmpStage);


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
        showStage(showName);
        getStage(closeName).close();
    }

    public void showDialog(String name, String resource, String ownerName)
    {
        registerStage(name, resource);
        showDialog(name, resource, getStage(ownerName));
    }

    public void showDialog(String name, String resource, Window owner)
    {
        try {
            Stage stage = loadStage(name, resource);
            stage.initModality(Modality.WINDOW_MODAL);
            stage.initOwner(owner);
            stage.showAndWait();
        }catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    public void showAndWaitStage(String name)
    {
        getStage(name).showAndWait();
    }

    public void showAndWaitStage(String showName, String closeName)
    {
        getStage(showName).showAndWait();
        getStage(closeName).close();
    }

    public void deregisterStage(String name)
    {
        Stage tmpStage = stages.remove(name);
        if(tmpStage == null)
            throw new RuntimeException("Stage " + name + " does not exists");
        else
            tmpStage.close();
    }
}
