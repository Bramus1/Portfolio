package view.javaFX;

import javafx.geometry.Pos;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.util.Pair;
import model.Game;

import java.util.ArrayList;

/**
 * BoardPane class is used to represent the graphical game board.
 */
public class BoardPane extends GridPane {


    private int circleSize;
    private int cellHeight;
    private int cellWidth;

    /**
     * Constructor for the BoardPane class.
     */
    public BoardPane() {

        setAlignment(Pos.TOP_LEFT);

        setHgap(1);
        setVgap(1);
        setStyle("-fx-background-color: green");
    }

    /**
     * Method to render the board with colored cells and pieces.
     *
     * @param game
     */
    public void renderBoard(Game game) {
        getChildren().clear(); // Clear previous rendering

        int boardHeight = game.getBoard().getHeight();
        int boardWidth = game.getBoard().getWidth();

        // Calculate the maximum size for the circles so that the board is not too large
        int maxSize = Math.min(30, Math.min(800 / boardHeight, 600 / boardWidth));

        // Calculate the size of the circles based on the size of the board
        double circleSize = maxSize / 2.0; // Divided by 2 to get an adequate margin
        this.setCircleSize((int) circleSize);
        // Iterate over the board to add the rectangles
        for (int row = 0; row < boardHeight; row++) {
            for (int col = 0; col < boardWidth; col++) {
                Rectangle rectangle = new Rectangle(circleSize * 2, circleSize * 2);
                rectangle.setFill(Color.GREEN);
                rectangle.setStroke(Color.BLACK);
                add(rectangle, col, row);
                this.setCellHeight((int) rectangle.getHeight());
                this.setCellWidth((int) rectangle.getWidth());


            }
        }

        // Iterate over the board to add the circles
        for (int i = 0; i < boardHeight; i++) {
            for (int j = 0; j < boardWidth; j++) {
                Circle circle = new Circle(circleSize);
                if (game.getBoard().getBoard()[i][j] == null) {
                    circle.setFill(Color.TRANSPARENT);
                } else if (game.getBoard().getBoard()[i][j].getColor() == model.Color.WHITE) {
                    circle.setFill(Color.WHITE);
                } else {
                    circle.setFill(Color.BLACK);
                }
                add(circle, j, i); // Nota l'ordine: (column, row)
            }
        }
    }

    /**
     * Method to update the board.
     *
     * @param game the game to update the board
     */
    public void updateBoard(Game game) {
        renderBoard(game);
        highlightPossibleMoves(game);

    }

    /**
     * Method to highlight the possible moves on the board.
     *
     * @param game the game to highlight the possible moves
     */
    public void highlightPossibleMoves(Game game) {
        if (game.getCurrentPlayer().getColor() == model.Color.WHITE) {
            highlightWhitePossibleMoves(game);
        } else {
            highlightBlackPossibleMoves(game);
        }


    }

    /**
     * Method to highlight the possible moves for the white player.
     *
     * @param game the game to highlight the possible moves for the white player
     */
    private void highlightWhitePossibleMoves(Game game) {
        ArrayList<Pair<Integer, Integer>> possibleMoves = game.getBoard().whitePossibleMovesList();

        for (Pair<Integer, Integer> move : possibleMoves) {
            Circle circle = new Circle(circleSize);
            circle.setFill(Color.GRAY);
            add(circle, move.getValue(), move.getKey());
        }
    }

    /**
     * Method to highlight the possible moves for the black player.
     *
     * @param game the game to highlight the possible moves for the black player
     */
    private void highlightBlackPossibleMoves(Game game) {
        ArrayList<Pair<Integer, Integer>> possibleMoves = game.getBoard().blackPossibleMovesList();

        for (Pair<Integer, Integer> move : possibleMoves) {
            Circle circle = new Circle(circleSize);
            circle.setFill(Color.GRAY);
            add(circle, move.getValue(), move.getKey());
        }
    }

    /**
     * Method to get the size of the circle.
     *
     * @return the size of the circle
     */
    public int getCellHeight() {

        return cellHeight;
    }

    /**
     * Method to get the width of the cell.
     *
     * @return the width of the cell
     */
    public int getCellWidth() {
        return cellWidth;
    }

    /**
     * Method to get the size of the circle.
     *
     * @param circleSize the size of the circle
     */
    public void setCircleSize(int circleSize) {
        this.circleSize = circleSize;
    }

    /**
     * Method to get the size of the circle.
     *
     * @param cellHeight the size of the circle
     */
    public void setCellHeight(int cellHeight) {
        this.cellHeight = cellHeight;
    }

    /**
     * Method to get the size of the circle.
     *
     * @param cellWidth the size of the circle
     */
    public void setCellWidth(int cellWidth) {
        this.cellWidth = cellWidth;
    }


}
