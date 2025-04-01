package model;

import Observer.Observable;
import strategy.Strategy;

import java.util.Objects;

/**
 * Game class is used to represent the game. It contains the board, the current player, the current piece, and the players.
 */
public class Game extends Observable {

    private Board board;

    private Piece currentPiece;
    private Player currentPlayer;
    private boolean gameOver;
    private Player playerBlack;
    private Player playerWhite;
    private Strategy currentStrategy;

    /**
     * Constructor for the Game class that takes a board as a parameter. It initializes the game with the board.
     * The current player is set to black and the current piece is set to black.
     * The players are initialized with the colors black and white.
     * The scores of the players are set to 2.
     * The game is not over.
     * The current strategy is set to null.
     * The game is observed by the view.
     *
     * @param board the board to be used in the game
     */
    public Game(Board board) {

        Objects.requireNonNull(board, "you need a board");
        this.board = board;
        this.gameOver = false;

        currentPiece = new Piece(Color.BLACK);
        playerBlack = new Player(Color.BLACK);
        playerWhite = new Player(Color.WHITE);
        this.currentPlayer = playerBlack;
        playerWhite.Score(2);
        playerBlack.Score(2);
        this.currentStrategy = null;


    }

    /**
     * Method to set the strategy of the game.
     * The strategy is used to determine the next move of the game.
     *
     * @param strategy
     */
    public void setStrategy(Strategy strategy) {
        this.currentStrategy = strategy;
    }

    /**
     * Method to get the strategy of the game.
     *
     * @return the strategy of the game
     */
    public Strategy getStrategy() {
        return currentStrategy;
    }

    /**
     * Method to get the board of the game.
     *
     * @return the board of the game
     */
    public Board getBoard() {
        return board;
    }

    /**
     * Method to change the current player.
     * If the current player is black, it changes to white and vice versa.
     */

    public void changePlayer() {
        if (currentPlayer.getColor() == Color.BLACK) {
            currentPlayer = playerWhite;
            currentPiece = new Piece(Color.WHITE);
        } else {
            currentPlayer = playerBlack;
            currentPiece = new Piece(Color.BLACK);
        }

    }

    /**
     * Method to get the current player.
     *
     * @return the current player
     */
    public Player getCurrentPlayer() {
        return currentPlayer;
    }


    /**
     * Method to add a piece to the board.
     *
     * @param x the x coordinate of the piece
     * @param y the y coordinate of the piece
     * @throws Exception if the piece cannot be added to the board
     */

    public void addPiece(int x, int y) throws Exception {

        board.addPiece(currentPiece, x, y);
        scoreCurrentPlayer();

        changePlayer();
        notifyObserver(this);

    }

    /**
     * Method to check if the game is over.
     *
     * @return true if the game is over, false otherwise
     */
    public boolean isGameOver() {
        if (board.blackPossibleMovesList().isEmpty() && board.whitePossibleMovesList().isEmpty()) {
            setGameOver(true);
            return true;
        } else {
            return false;
        }
    }

    /**
     * Method to score the current player.
     *
     * @return the score of the current player as a string
     */
    public String scoreCurrentPlayer() {
        if (currentPlayer.getColor() == Color.BLACK) {
            int score = board.countPieces(Color.BLACK);
            playerBlack.Score(score);
            currentPlayer.Score(score);
        } else {
            int score = board.countPieces(Color.WHITE);
            playerWhite.Score(score);
            currentPlayer.Score(score);
        }


        return Integer.toString(currentPlayer.getScore());
    }

    /**
     * Method to remove a piece from the board.
     *
     * @param x the x coordinate of the piece
     * @param y the y coordinate of the piece
     */
    public void removePiece(int x, int y) {
        board.removePiece(x, y);
    }

    /**
     * Method to set the game over.
     *
     * @param gameOver the state of the game
     */
    public void setGameOver(boolean gameOver) {
        this.gameOver = gameOver;

    }

    /**
     * method to check if the current player has moves.
     *
     * @param current the current player
     * @return true if the current player has moves, false otherwise
     */
    public boolean hasMoves(Player current) {
        if (current.getColor() == Color.BLACK) {
            return !board.blackPossibleMovesList().isEmpty();
        } else {
            return !board.whitePossibleMovesList().isEmpty();

        }
    }

    /**
     * Method to get the player with the black color.
     *
     * @return the player with the black color
     */
    public Player getPlayerBlack() {
        return playerBlack;
    }

    /**
     * Method to get the player with the white color.
     *
     * @return the player with the white color
     */
    public Player getPlayerWhite() {
        return playerWhite;
    }

    /**
     * Method to get the game over state.
     *
     * @return the game over state
     */
    public boolean getGameOver() {
        return gameOver;
    }

    /**
     * Method to redo the last move.
     */
    public void redo() {
        board.redoMove();
        changePlayer();
        notifyObserver(this);


    }

    /**
     * Method to undo the last move.
     */
    public void undo() {
        board.undoMove();
        changePlayer();

        notifyObserver(this);
    }

    /**
     * Method to restart the game.
     */
    public void restart() {
        board.restart();
        playerBlack.Score(2);
        playerWhite.Score(2);
        currentPlayer = playerBlack;
        currentPiece = new Piece(Color.BLACK);
        gameOver = false;
        notifyObserver(this);
    }


}
