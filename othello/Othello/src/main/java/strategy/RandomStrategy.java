package strategy;

import controller.AddPiece;
import controller.Commands;
import javafx.util.Pair;
import model.Color;
import model.Game;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Random;

/**
 * RandomStrategy class is used to represent the random strategy of the game.
 */
public class RandomStrategy implements Strategy {

    private final LinkedList<Commands> undoHistory;
    private final LinkedList<Commands> redoHistory;

    /**
     * Constructor for the RandomStrategy class that initializes the undo and redo history
     */
    public RandomStrategy() {
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
        // Collect all possible shots

        ArrayList<Pair<Integer, Integer>> possibleMovesWhite = game.getBoard().whitePossibleMovesList();

        if (!game.hasMoves(game.getCurrentPlayer())) {

            game.changePlayer();
            return;

        } else {
            Random random = new Random();
            if (game.getCurrentPlayer().getColor() == Color.WHITE) {

                Pair<Integer, Integer> randomMove = possibleMovesWhite.get(random.nextInt(possibleMovesWhite.size()));
                AddPiece addPiece = new AddPiece(randomMove.getKey(), randomMove.getValue(), game);
                addPiece.execute();
                undoHistory.add(addPiece);

            }


        }

    }

    /**
     * Method to undo the move of the game.
     *
     * @param game the game to undo the move
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
     * @param game the game to redo the move
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
