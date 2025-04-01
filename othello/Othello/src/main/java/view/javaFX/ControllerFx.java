package view.javaFX;

import controller.Commands;
import model.Color;
import model.Game;

import java.util.LinkedList;
import java.util.Objects;

/**
 * ControllerFx class is used to represent the controller of the game in the JavaFX view.
 */
public class ControllerFx {
    private final Game game;
    private final MainPane mainPane;

    private final LinkedList<Commands> undoHistory;
    private final LinkedList<Commands> redoHistory;

    /**
     * Constructor for the ControllerFx class that initializes the game, mainPane, undoHistory and redoHistory
     *
     * @param game     the game to initialize
     * @param mainPane the mainPane to initialize
     */
    public ControllerFx(Game game, MainPane mainPane) {
        Objects.requireNonNull(game, " you need a game");
        Objects.requireNonNull(mainPane, " you need a mainPane");

        this.game = game;
        this.mainPane = mainPane;
        this.undoHistory = new LinkedList<>();
        this.redoHistory = new LinkedList<>();
        game.addObserver(mainPane);

    }

    /**
     * method to initialize the controllerFx
     *
     * @param controllerFx the controllerFx to initialize
     * @throws Exception if the move is invalid
     */
    public void initialise(ControllerFx controllerFx) throws Exception {

        mainPane.initialise(controllerFx);
    }

    /**
     * method to undo the last move made by the player
     * if the undo history is empty, it will print a message
     */
    public void undo() {

        if (undoHistory.isEmpty()) {
            System.out.println("No more undo from fx");

        } else {
            Commands command = undoHistory.removeLast();
            try {
                command.undo();
                redoHistory.add(command);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }


    /**
     * method to redo the last move made by the player
     * if the redo history is empty, it will print a message
     */
    public void redo() {


        if (redoHistory.isEmpty()) {
            System.out.println("No more redo from fx");
        } else {
            Commands command = redoHistory.removeLast();
            try {
                command.redo();
                undoHistory.add(command);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * method to add a piece to the board and add the command to the undo history list to keep track of the moves made by the player
     *
     * @param command the command to execute
     * @throws Exception if the move is not valid
     */
    public void addPieceCommand(Commands command) throws Exception {

        command.execute();
        undoHistory.push(command);
        redoHistory.clear(); // clear the redo history list when a new move is made by the player

    }

    /**
     * method to play vs the strategy of the game
     *
     * @throws Exception if the move is not valid
     */
    public void playVsStrategy() throws Exception {
        if (game.getCurrentPlayer().getColor() == Color.WHITE && game.getStrategy() != null) {
            game.getStrategy().makeMove(game);


        }


    }

    /**
     * method to undo the move of the strategy.
     *
     * @throws Exception if the move is invalid
     */
    public void undoStrategy() throws Exception {
        if (game.getCurrentPlayer().getColor() == Color.WHITE && game.getStrategy() != null) {

            game.getStrategy().undoMove(game);

        }
    }

    /**
     * method to redo the move of the strategy.
     *
     * @throws Exception if the move is invalid
     */
    public void redoStrategy() throws Exception {
        if (game.getCurrentPlayer().getColor() == Color.WHITE && game.getStrategy() != null) {

            game.getStrategy().redoMove(game);

        }
    }

    /**
     * method to surrender from the game
     * set the game over to true
     */
    public void surrender() {

        game.setGameOver(true);


    }

    /**
     * method to get the game
     *
     * @return the game
     */
    public Game getGame() {
        return game;
    }


}