package strategy;

import model.Game;

/**
 * Strategy interface is used to represent the strategy of the game.
 */
public interface Strategy {
    /**
     * Method to make a move in the game.
     *
     * @param game the game to make a move in
     * @throws Exception if the move is invalid
     */
    void makeMove(Game game) throws Exception;

    /**
     * Method to undo a move in the game.
     *
     * @param game the game to undo the move in
     * @throws Exception if the move is invalid
     */
    void undoMove(Game game) throws Exception;

    /**
     * Method to redo a move in the game.
     *
     * @param game the game to redo the move in
     * @throws Exception if the move is invalid
     */
    void redoMove(Game game) throws Exception;
}
