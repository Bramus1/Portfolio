package strategy;

import controller.AddPiece;
import controller.Commands;
import javafx.util.Pair;
import model.Color;
import model.Game;
import model.Piece;

import java.util.ArrayList;
import java.util.LinkedList;

/**
 * CleverStrategy class is used to represent the clever strategy of the game.
 * It is used to determine the next move of the game.
 * The clever strategy tries to make the best move possible.
 */
public class CleverStrategy implements Strategy {

    private final LinkedList<Commands> undoHistory;
    private final LinkedList<Commands> redoHistory;

    public CleverStrategy() {
        this.undoHistory = new LinkedList<>();
        this.redoHistory = new LinkedList<>();
    }


    /**
     * Method to make the move of the game.
     *
     * @param game the game to make the move
     * @throws Exception if the move is invalid
     */
    @Override
    public void makeMove(Game game) throws Exception {

        // i need to make this clever strategy
        // i will try to make the best move possible
        // i will try to make the move that will flip the most pieces
        // i will try to make the move that will give me the most pieces

        // Collect all possible shots
        ArrayList<Pair<Integer, Integer>> possibleMovesWhite = game.getBoard().whitePossibleMovesList();
        int maxCaptures = 0;
        Pair<Integer, Integer> bestMove = null;
        // if the player has no moves then change the player
        if (!game.hasMoves(game.getCurrentPlayer())) {

            game.changePlayer();
            return;
        }
        if (game.getCurrentPlayer().getColor() == Color.WHITE) {
            for (Pair<Integer, Integer> move : possibleMovesWhite) {
                int captures = calculateCaptures(game, move); // Calculer the captures
                if (captures > maxCaptures) { // Choice the best move
                    maxCaptures = captures; // Update the max captures
                    bestMove = move; // Update the best move
                }
            }
            AddPiece addPiece = new AddPiece(bestMove.getKey(), bestMove.getValue(), game);
            addPiece.execute();
            undoHistory.add(addPiece);

        }


    }

    /**
     * Method to calculate the captures of the game.
     *
     * @param game the game to calculate the captures
     * @param move the move to calculate the captures
     * @return the captures of the game
     */
    private int calculateCaptures(Game game, Pair<Integer, Integer> move) {
        int captures = 0;
        int x = move.getKey();
        int y = move.getValue();

        // I check all the directions
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                // I check if the direction is valid
                if (dx == 0 && dy == 0) {
                    continue;
                }
                captures += countCapturesInDirection(game, x, y, dx, dy); // I count the captures in a direction in the game recursively
            }
        }

        return captures;
    }

    /**
     * Method to count the captures in a direction.
     *
     * @param game the game to count the captures in a direction
     * @param x    the x coordinate of the piece
     * @param y    the y coordinate of the piece
     * @param dx   the x direction of the piece
     * @param dy   the y direction of the piece
     * @return the captures in a direction
     */
    private int countCapturesInDirection(Game game, int x, int y, int dx, int dy) {
        int captures = 0;
        int i = x + dx;
        int j = y + dy;

        while (i >= 0 && i < game.getBoard().getHeight() && j >= 0 && j < game.getBoard().getWidth()) {  // I check if the piece is in the board
            Piece piece = game.getBoard().getPiece(i, j);
            if (piece == null) {
                return 0;
            }
            if (piece.getColor() == game.getCurrentPlayer().getColor()) {
                return captures;
            }
            captures++;
            i += dx;
            j += dy;
        }

        return 0;
    }

    /**
     * Method to undo the move of the game.
     *
     * @param game the game to undo the move in the game
     * @throws Exception if the move is invalid
     */
    @Override
    public void undoMove(Game game) throws Exception {

        if (!undoHistory.isEmpty()) {
            Commands command = undoHistory.removeLast();
            command.undo();
            redoHistory.add(command);
        }


    }

    /**
     * Method to redo the move of the game.
     *
     * @param game the game to redo the move in the game
     * @throws Exception if the move is invalid
     */
    @Override
    public void redoMove(Game game) throws Exception {

        if (!redoHistory.isEmpty()) {
            Commands command = redoHistory.removeLast();
            command.redo();
            undoHistory.add(command);
        }

    }
}
