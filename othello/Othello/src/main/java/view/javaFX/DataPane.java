package view.javaFX;

import javafx.geometry.Pos;
import javafx.scene.layout.HBox;
import model.Game;

/**
 * DataPane class is used to represent the data pane of the game.
 * it contains the board pane and the info pane
 */
public class DataPane extends HBox {


    private BoardPane boardPane;
    private InfoPane infoPane;

    /**
     * Constructor for the DataPane class.
     */
    public DataPane() {
        boardPane = new BoardPane();
        infoPane = new InfoPane();

        this.getChildren().addAll(boardPane, infoPane);
        this.setAlignment(Pos.CENTER);
        this.setSpacing(10);
    }

    /**
     * Method to get the board pane.
     *
     * @return the board pane
     */
    public BoardPane getBoardPane() {
        return boardPane;
    }

    /**
     * Method to get the info pane.
     *
     * @return the info pane
     */
    public InfoPane getInfoPane() {
        return infoPane;
    }

    /**
     * Method to render the board with colored cells and pieces.
     *
     * @param game the game to be rendered
     */
    public void renderBoard(Game game) {
        boardPane.renderBoard(game);
    }

    /**
     * Method to update the board with the new game.
     *
     * @param game the game to be updated
     */
    public void updateBoard(Game game) {
        boardPane.updateBoard(game);
        infoPane.updateInfo(game);
    }

    /**
     * Method to set the player black score.
     *
     * @param game the game to set the player black score
     */
    public void setPlayerBlackScore(Game game) {
        infoPane.setPlayerBlackScore(game);
    }

    /**
     * Method to set the player white score.
     *
     * @param game the game to set the player white score
     */
    public void setPlayerWhiteScore(Game game) {
        infoPane.setPlayerWhiteScore(game);
    }

}
