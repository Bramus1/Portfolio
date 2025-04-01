package view.console;

import Observer.Observer;
import model.Game;
import model.Piece;

import java.util.Objects;
import java.util.Scanner;

/**
 * View class is used to display the board and the game status. It is also used to get input from the user.
 * It implements the Observer interface to be able to observe the game.
 * It is used by the controller to display the board and the game status.
 */
public class View implements Observer {

    /**
     * Constructor for the View class that takes a game as a parameter.
     *
     * @param game the game to be displayed and observed by the view
     */
    public View(Game game) {
        Objects.requireNonNull(game, "you need a game");

        game.addObserver(this);

    }

    /**
     * Method to update the view when the game is updated.
     *
     * @param obj the object to be updated
     */
    @Override
    public void update(Object obj) {
        Game game = (Game) obj;
        displayBoard(game.getBoard().getBoard());

        System.out.println("Player " + ((Game) obj).getCurrentPlayer().toString() + "'s turn." + "\n Your score is : " + ((Game) obj).scoreCurrentPlayer());


    }

    /**
     * Method to get the input from the user.
     *
     * @param minVal the minimum value of the input
     * @param maxVal the maximum value of the input
     * @return the valid input from the user
     */
    public static int getValidInput(int minVal, int maxVal) {
        Scanner scanner = new Scanner(System.in);
        int input;
        boolean validInput = false;
        do {
            while (!scanner.hasNextInt()) {
                System.out.println("Invalid input. Please enter a valid number.");
                scanner.next(); // Discard invalid input
            }
            input = scanner.nextInt();

            if (input < minVal || input > maxVal) {
                System.out.println("Invalid input. Please enter a number between " + minVal + " and " + maxVal + ".");
            } else {
                validInput = true;
            }
        } while (!validInput);

        return input;
    }

    /**
     * Method to display the board.
     *
     * @param board the board to be displayed
     */
    public void displayBoard(Piece[][] board) {
        System.out.print("    ");
        for (int i = 0; i < board[0].length; i++) {
            if (i >= 10) {
                System.out.print(i + "  ");
            } else {
                System.out.print(" " + i + "  ");
            }
        }
        System.out.println();

        for (int i = 0; i < board.length; i++) {
            if (i >= 10) {
                System.out.print(i + " |");
            } else {
                System.out.print(" " + i + " |");
            }
            for (int j = 0; j < board[i].length; j++) {
                if (board[i][j] == null) {
                    System.out.print(" - |");
                } else {
                    System.out.print(" " + board[i][j].toString() + "|");
                }
            }

            System.out.print("  ");
            for (int k = 0; k < board[0].length; k++) {
            }
            System.out.println();
        }
    }

    /**
     * Method to display the possible moves for the player.
     *
     * @param game the game to display the possible moves for the player
     */
    public void suggestionPositionPlayer(Game game) {

        if (game.getCurrentPlayer().toString().equals("BLACK")) {
            System.out.println("Possible moves for player BLACK: ");
            ;
            System.out.println(game.getBoard().blackPossibleMovesList().toString());
        } else {
            System.out.println("Possible moves for player WHITE: ");

            System.out.println(game.getBoard().whitePossibleMovesList().toString());

        }
    }

    /**
     * Method to display the possible moves for the player.
     *
     * @return the input from the user
     */
    public String ask() {
        System.out.println("Enter the coordinates of the piece you want to place (y x) or type 'exit' to end the game: ");
        Scanner scanner = new Scanner(System.in);
        return scanner.nextLine();
    }

    /**
     * Method to display the winner of the game.
     *
     * @param game the game to display the winner
     */
    public void displayWinner(Game game) {
        if (game.getPlayerBlack().getScore() > game.getPlayerWhite().getScore()) {
            System.out.println("The winner is player BLACK with a score of : " + game.getPlayerBlack().getScore());
        } else if (game.getPlayerBlack().getScore() < game.getPlayerWhite().getScore()) {
            System.out.println("The winner is player WHITE with a score of : " + game.getPlayerWhite().getScore());
        } else {
            System.out.println("It's a draw");

        }

    }

    /**
     * Method to display the current player.
     *
     * @param game the game to display the current player
     */
    public void currentPlayer(Game game) {
        System.out.println("Player " + game.getCurrentPlayer().toString() + "'s turn.");
    }

    /**
     * Method to display the score of the player.
     */
    public void askStrategy() {
        System.out.println("Choose a strategy: ");
        System.out.println("1. Random");
        System.out.println("2. Clever");

    }

    /**
     * Method to display the type of game.
     */
    public void askTypeGame() {
        System.out.println("Choose a type of game: ");
        System.out.println("1. Human vs Human");
        System.out.println("2. Human vs Computer");
    }


}

