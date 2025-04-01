import controller.Controller;
import model.Board;
import model.Game;
import strategy.RandomStrategy;
import strategy.Strategy;
import view.console.View;

import java.util.Scanner;

public class OthelloLauncherConsole {


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

            // input = scanner.nextInt();
            if (input < minVal || input > maxVal) {
                System.out.println("Invalid input. Please enter a number between " + minVal + " and " + maxVal + ".");
            } else {
                validInput = true;
            }
        } while (!validInput);

        //  scanner.close();
        return input;
    }


    public static void main(String[] args) {

        System.out.println("Welcome to the Othello game ");
        int height, width;

        System.out.print("Enter the height  of the board : ");

        height = getValidInput(3, 15);
        System.out.print("Enter the width of  the board  : ");
        width = getValidInput(3, 15);


        Board board = new Board(height, width);

       Game game = new Game(board);

        View view = new View(game);
        Controller controller = new Controller(game, view);
        try {

            controller.start();

        } catch (Exception e) {
            e.printStackTrace();


        }

    }
}
