package view.javaFX;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.control.MenuBar;

/**
 * The MenuB class creates a menu bar with an exit menu item.
 */
public class MenuB extends MenuBar {
    private Exit exit;
    private EventHandler<ActionEvent> exitHandler;


    /**
     * Constructs a MenuB object and initializes the exit menu item.
     */
    public MenuB() {
        exit = new Exit();
        this.getMenus().add(exit);
        exitHandler = new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent actionEvent) {
                Platform.exit();
            }
        };
        exit.setOnAction(exitHandler);


    }

}
