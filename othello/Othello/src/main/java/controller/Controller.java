package controller;

import model.Color;
import model.Game;
import strategy.CleverStrategy;
import strategy.RandomStrategy;
import strategy.Strategy;
import view.console.View;
import view.javaFX.MainPane;

import java.util.LinkedList;
import java.util.Objects;

/**
 * Controller class that manages the game and the view of the game and the interaction between the two
 */
public class Controller {
    private final Game game;
    private MainPane mainPane;
    private View view;

    private LinkedList<Commands> undoHistory;
    private LinkedList<Commands> redoHistory;

    /**
     * Constructor for the Controller class that takes a game and a view as parameters and initializes the undo and redo history
     *
     * @param game the game object
     * @param view the view object
     */
    public Controller(Game game, View view) {
        Objects.requireNonNull(game, " you need a game");
        Objects.requireNonNull(view, " you need a view");
        this.view = view;
        this.game = game;
        this.undoHistory = new LinkedList<>();
        this.redoHistory = new LinkedList<>();


    }

    /**
     * method to play Otello with two players and manage the game flow based on the player's interactions
     *
     * @throws Exception
     */
    public void twoPlayer() throws Exception {

        view.currentPlayer(game);


        view.displayBoard(game.getBoard().getBoard());


        while (!game.isGameOver() && !game.getGameOver()) {

            String line = "";
            int x, y;

            try {

                line = view.ask();
                String[] split = line.split("\\s+");
                switch (split[0].toLowerCase()) {
                    case "undo":
                        undo();

                        break;
                    case "redo":
                        redo();
                        break;
                    case "exit":
                        game.setGameOver(true);
                        break;
                    case "help":
                        view.suggestionPositionPlayer(game);
                        break;
                    default:
                        x = Integer.parseInt(split[0]);
                        y = Integer.parseInt(split[1]);
                        AddPiece addPiece = new AddPiece(x, y, game);
                        addPieceCommand(addPiece);

                        break;


                }
                if (!game.hasMoves(game.getCurrentPlayer())) {
                    System.out.println("No more moves for player " + game.getCurrentPlayer().toString());
                    game.changePlayer();
                    game.notifyObserver(game);
                }


            } catch (Exception e) {
                System.out.println(e.getMessage());

            }


        }


        view.displayWinner(game);

    }

    /**
     * method to undo the last move made by the player
     * if the undo history is empty, it will print a message
     */
    public void undo() {

        if (undoHistory.isEmpty()) {
            System.out.println("No more undo");

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
            System.out.println("No more redo");
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
     * and allow the player to undo and redo the moves made during the game
     *
     * @param addPiece the command to add a piece to the board and update the game state accordingly to the move made by the player
     * @throws Exception if the move is not valid
     */
    public void addPieceCommand(Commands addPiece) throws Exception {
        addPiece.execute();
        undoHistory.add(addPiece);
        redoHistory.clear();
    }

    /**
     * method to play against the computer with a strategy chosen by the player
     * the player can choose between a random strategy and a clever strategy
     * the game will display the board and the player will be able to make a move by entering the coordinates
     * of the piece he wants to place on the board
     *
     * @throws Exception if the move is not valid
     */
    private void playVsStrategy() throws Exception {
        Strategy strategy;
        view.askStrategy();
        int choice = view.getValidInput(1, 2);
        switch (choice) {
            case 1:
                strategy = new RandomStrategy();
                break;
            case 2:
                strategy = new CleverStrategy();
                break;
            default:
                throw new IllegalStateException("Unexpected value: " + choice);
        }
        view.displayBoard(game.getBoard().getBoard());
        game.setStrategy(strategy);

        while (!game.isGameOver() && !game.getGameOver()) {
            if (game.getCurrentPlayer().getColor() == Color.WHITE) {
                game.getStrategy().makeMove(game);


            } else {
                String line = "";
                int x, y;

                try {

                    if (!game.hasMoves(game.getCurrentPlayer())) {
                        System.out.println("No more moves for player " + game.getCurrentPlayer().toString());
                        game.changePlayer();
                        game.notifyObserver(game);
                        continue;
                    }


                    line = view.ask();
                    String[] split = line.split("\\s+");
                    switch (split[0].toLowerCase()) {
                        case "undo":
                            undo();
                            game.getStrategy().undoMove(game);

                            break;
                        case "redo":
                            redo();
                            game.getStrategy().redoMove(game);
                            break;
                        case "exit":
                            game.setGameOver(true);
                            break;
                        case "help":
                            view.suggestionPositionPlayer(game);
                            break;
                        default:
                            x = Integer.parseInt(split[0]);
                            y = Integer.parseInt(split[1]);
                            AddPiece addPiece = new AddPiece(x, y, game);
                            addPieceCommand(addPiece);

                            break;


                    }


                } catch (Exception e) {
                    System.out.println(e.getMessage());

                }


            }
        }
        view.displayWinner(game);
    }

    /**
     * method to start the game and allow the player to choose between playing against another player or against the computer
     * with a strategy chosen by the player
     * the game will display the board and the player will be able to make a move by entering the coordinates
     *
     * @throws Exception if the move is not valid
     */
    public void start() throws Exception {

        view.currentPlayer(game);
        view.displayBoard(game.getBoard().getBoard());
        view.askTypeGame();
        int choice = view.getValidInput(1, 2);
        if (choice == 1) {
            twoPlayer();
        } else {
            playVsStrategy();
        }


    }


}