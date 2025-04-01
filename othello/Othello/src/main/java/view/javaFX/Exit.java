package view.javaFX;

import javafx.scene.control.Menu;
import javafx.scene.control.MenuItem;

/**
 * The Exit class creates a menu with an exit menu item.
 */
public class Exit extends Menu {
    private MenuItem exitMenuItem;

    /**
     * Constructs an Exit object and initializes the exit menu item.
     */
    public Exit() {
        super("Menu");
        exitMenuItem = new MenuItem("Exit");
        this.getItems().add(exitMenuItem);
    }

}
