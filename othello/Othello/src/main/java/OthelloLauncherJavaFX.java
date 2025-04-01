import controller.Controller;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import model.Board;
import model.Game;
import view.javaFX.ControllerFx;
import view.javaFX.MainPane;
import view.javaFX.SettingsGame;

public class OthelloLauncherJavaFX extends Application {
    @Override
    public void start(Stage stage) throws Exception {
        //stage.setTitle("Othello");
        // i do this just to have  a board to pass to the game
        Board board = new Board(10, 10);
        // i do this just to have a game to pass to the mainPane
        Game game = new Game(board);
        MainPane othelloPane = new MainPane(stage, game);

        SettingsGame settingsGame = new SettingsGame(othelloPane);
        settingsGame.changeScene(stage);

         stage.show();

    }
}
