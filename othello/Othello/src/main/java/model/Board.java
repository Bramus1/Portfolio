package model;

import javafx.util.Pair;

import java.util.ArrayList;
import java.util.Stack;

/**
 * This class represents the board of the game
 */
public class Board {
    private int height;
    private int width;

    private Piece[][] board;
    private Stack<Piece[][]> undoStack;
    private Stack<Piece[][]> redoStack;

    /**
     * Constructor for the board class that initializes the board with the specified height and width
     *
     * @param height height of the board
     * @param width  width of the board
     */
    public Board(int height, int width) {
        this.height = height;
        this.width = width;
        undoStack = new Stack<>();
        redoStack = new Stack<>();
        this.board = new Piece[height][width];

        board[height / 2][width / 2] = new Piece(Color.WHITE);
        board[height / 2 - 1][width / 2 - 1] = new Piece(Color.WHITE);
        board[height / 2 - 1][width / 2] = new Piece(Color.BLACK);
        board[height / 2][width / 2 - 1] = new Piece(Color.BLACK);


    }

    /**
     * get the piece at the specified coordinates
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return the piece at the specified coordinates
     */
    public Piece getPiece(int x, int y) {
        return board[x][y];
    }

    /**
     * get the height of the board
     *
     * @return the height of the board
     */
    public int getHeight() {
        return height;
    }

    /**
     * get the width of the board
     *
     * @return the width of the board
     */
    public int getWidth() {
        return width;
    }

    /**
     * get the board of the game
     *
     * @return the board of the game
     */
    public Piece[][] getBoard() {
        return board;
    }


    /**
     * clear the board
     */
    public void clear() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                board[i][j] = null;
            }
        }

    }

    /**
     * flip the pieces in the specified direction going up
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesUp(int x, int y, Color color) {

        for (int i = x - 1; i >= 0; i--) {

            if (board[i][y] == null) {
                break;
            }
            board[i][y].setColor(color);


        }

    }

    /**
     * flip the pieces in the specified direction going down
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesDown(int x, int y, Color color) {


        for (int i = x + 1; i < height; i++) {

            if (board[i][y] == null) {
                break;
            }


            board[i][y].setColor(color);


        }


    }

    /**
     * flip the pieces in the specified direction going left from the specified coordinates
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesLeft(int x, int y, Color color) {


        for (int i = y - 1; i >= 0; i--) {
            if (board[x][i] == null) {
                break;
            }
            board[x][i].setColor(color);


        }


    }

    /**
     * flip the pieces in the specified direction going right from the specified coordinates
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesRight(int x, int y, Color color) {

        for (int i = y + 1; i < width; i++) {
            if (board[x][i] == null) {
                break;
            }

            board[x][i].setColor(color);


        }

    }

    /**
     * flip the pieces in the specified direction going up left from the specified coordinates
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesUpLeft(int x, int y, Color color) {


        for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
            if (board[i][j] == null) {
                break;
            }
            board[i][j].setColor(color);


        }

    }

    /**
     * flip the pieces in the specified direction going up right from the specified coordinates
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesUpRight(int x, int y, Color color) {


        for (int i = x - 1, j = y + 1; i >= 0 && j < width; i--, j++) {
            if (board[i][j] == null) {
                break;
            }
            board[i][j].setColor(color);


        }


    }

    /**
     * flip the pieces in the specified direction going down left from the specified coordinates
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesDownLeft(int x, int y, Color color) {

        for (int i = x + 1, j = y - 1; i < height && j >= 0; i++, j--) {
            if (board[i][j] == null) {
                break;
            }
            board[i][j].setColor(color);


        }

    }

    /**
     * flip the pieces in the specified direction going down right from the specified coordinates
     *
     * @param x     x coordinate
     * @param y     y coordinate
     * @param color color of the piece
     */
    private void flipPiecesDownRight(int x, int y, Color color) {

        for (int i = x + 1, j = y + 1; i < height && j < width; i++, j++) {
            if (board[i][j] == null) {
                break;
            }
            board[i][j].setColor(color);


        }

    }

    /**
     * verify if the pieces can be flipped in the specified direction going up from the specified coordinates
     *
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */
    public boolean verifyUp(int x, int y, Color oppositeColor) {

        boolean result = false;
        if (x <= 0 || board[x][y] != null) {
            return false;

            //  return false; // No need to throw an exception, just return false
        }
        if (board[x - 1][y] == null) {
            return false;
        }

        Piece piece = board[x - 1][y];

        while (x > 0 && piece != null && piece.getColor() == oppositeColor) {
            x = x - 1;
            piece = board[x][y];


            if (piece != null && piece.getColor() != oppositeColor) {


                result = true;
                break;
            }
        }


        return result;
    }

    /**
     * verify if the pieces can be flipped in the specified direction going down from the specified coordinates
     *
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */
    public boolean verifyDown(int x, int y, Color oppositeColor) {
        boolean result = false;
        if (x >= board.length - 1 || board[x][y] != null) {
            return false;
        }
        if (board[x + 1][y] == null) {
            return false;
        }
        Piece piece = board[x + 1][y];
        while (x < board.length - 1 && piece != null && piece.getColor() == oppositeColor) {
            x = x + 1;
            piece = board[x][y];
            if (piece != null && piece.getColor() != oppositeColor) {
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * verify if the pieces can be flipped in the specified direction going left from the specified coordinates
     *
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */
    public boolean verifyLeft(int x, int y, Color oppositeColor) {
        boolean result = false;
        if (y <= 0 || board[x][y] != null) {
            return false;

        }
        if (board[x][y - 1] == null) {
            return false;
        }
        Piece piece = board[x][y - 1];
        while (y > 0 && piece != null && piece.getColor() == oppositeColor) {
            y = y - 1;
            piece = board[x][y];
            if (piece != null && piece.getColor() != oppositeColor) {
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * verify if the pieces can be flipped in the specified direction going right from the specified coordinates
     *
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */
    public boolean verifyRight(int x, int y, Color oppositeColor) {
        boolean result = false;
        if (y >= board[0].length - 1 || board[x][y] != null) {
            return false;

        }
        if (board[x][y + 1] == null) {
            return false;
        }
        Piece piece = board[x][y + 1];
        while (y < board[0].length - 1 && piece != null && piece.getColor() == oppositeColor) {
            y = y + 1;
            piece = board[x][y];
            if (piece != null && piece.getColor() != oppositeColor) {
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * verify if the pieces can be flipped in the specified direction going up left from the specified coordinates
     *
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */
    public boolean verifyUpLeft(int x, int y, Color oppositeColor) {
        boolean result = false;
        if (x <= 0 || y <= 0 || board[x][y] != null) {
            return false;

        }
        if (board[x - 1][y - 1] == null) {
            return false;
        }
        Piece piece = board[x - 1][y - 1];
        while (x > 0 && y > 0 && piece != null && piece.getColor() == oppositeColor) {
            x = x - 1;
            y = y - 1;
            piece = board[x][y];
            if (piece != null && piece.getColor() != oppositeColor) {
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * verify if the pieces can be flipped in the specified direction going up right from the specified coordinates
     *
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */
    public boolean verifyUpRight(int x, int y, Color oppositeColor) {
        boolean result = false;
        if (x <= 0 || y >= board[0].length - 1 || board[x][y] != null) {
            return false;

        }
        if (board[x - 1][y + 1] == null) {
            return false;
        }
        Piece piece = board[x - 1][y + 1];
        while (x > 0 && y < board[0].length - 1 && piece != null && piece.getColor() == oppositeColor) {
            x = x - 1;
            y = y + 1;
            piece = board[x][y];
            if (piece != null && piece.getColor() != oppositeColor) {
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * verify if the pieces can be flipped in the specified direction going down left from the specified coordinates
     *
     * @param x             x coordinate
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */

    public boolean verifyDownLeft(int x, int y, Color oppositeColor) {
        boolean result = false;
        if (x >= board.length - 1 || y <= 0 || board[x][y] != null) {
            return false;
        }
        if (board[x + 1][y - 1] == null) {
            return false;
        }
        Piece piece = board[x + 1][y - 1];
        while (x < board.length - 1 && y > 0 && piece != null && piece.getColor() == oppositeColor) {
            x = x + 1;
            y = y - 1;
            piece = board[x][y];
            if (piece != null && piece.getColor() != oppositeColor) {
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * verify if the pieces can be flipped in the specified direction going down right from the specified coordinates
     *
     * @param x             x coordinate
     * @param y             y coordinate
     * @param oppositeColor color of the opposite piece
     * @return true if the pieces can be flipped, false otherwise
     */
    public boolean verifyDownRight(int x, int y, Color oppositeColor) {
        boolean result = false;
        if (x >= board.length - 1 || y >= board[0].length - 1 || board[x][y] != null) {
            return false;

        }
        if (board[x + 1][y + 1] == null) {
            return false;
        }
        Piece piece = board[x + 1][y + 1];
        while (x < board.length - 1 && y < board[0].length - 1 && piece != null && piece.getColor() == oppositeColor) {
            x = x + 1;
            y = y + 1;
            piece = board[x][y];
            if (piece != null && piece.getColor() != oppositeColor) {
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * flip the pieces in the specified direction from the specified coordinates
     *
     * @param piece     the piece to be flipped
     * @param x         x coordinate
     * @param y         y coordinate
     * @param direction the direction of the pieces to be flipped
     */
    private void flipPiecesInDirection(Piece piece, int x, int y, Direction direction) {

        switch (direction) {
            case UP:
                flipPiecesUp(x, y, piece.getColor());
                break;
            case DOWN:
                flipPiecesDown(x, y, piece.getColor());
                break;
            case LEFT:
                flipPiecesLeft(x, y, piece.getColor());
                break;
            case RIGHT:
                flipPiecesRight(x, y, piece.getColor());
                break;
            case UPLEFT:
                flipPiecesUpLeft(x, y, piece.getColor());
                break;
            case UPRIGHT:
                flipPiecesUpRight(x, y, piece.getColor());
                break;
            case DOWNLEFT:
                flipPiecesDownLeft(x, y, piece.getColor());
                break;
            case DOWNRIGHT:
                flipPiecesDownRight(x, y, piece.getColor());
                break;

            default:
                throw new IllegalArgumentException("Invalid direction");
        }

    }

    /**
     * verify if the pieces can be flipped in the specified direction from the specified coordinates
     *
     * @param piece     the piece to be flipped
     * @param x         x coordinate
     * @param y         y coordinate
     * @param direction the direction of the pieces to be flipped
     * @return true if the pieces can be flipped, false otherwise
     * @throws Exception if the direction is invalid
     */

    private boolean willFlipPieces(Piece piece, int x, int y, Direction direction) throws Exception {
        switch (direction) {
            case UP:
                return verifyUp(x, y, piece.oppositeColor());
            case DOWN:
                return verifyDown(x, y, piece.oppositeColor());
            case LEFT:
                return verifyLeft(x, y, piece.oppositeColor());
            case RIGHT:
                return verifyRight(x, y, piece.oppositeColor());
            case UPLEFT:
                return verifyUpLeft(x, y, piece.oppositeColor());
            case UPRIGHT:
                return verifyUpRight(x, y, piece.oppositeColor());
            case DOWNLEFT:
                return verifyDownLeft(x, y, piece.oppositeColor());
            case DOWNRIGHT:
                return verifyDownRight(x, y, piece.oppositeColor());

            default:
                throw new IllegalArgumentException("Invalid direction");
        }
    }

    /**
     * add a piece to the board at the specified coordinates
     *
     * @param piece the piece to be added
     * @param x     x coordinate
     * @param y     y coordinate
     * @throws Exception if the move is not valid
     */
    public void addPiece(Piece piece, int x, int y) throws Exception {
        if (board[x][y] != null) {
            throw new IllegalArgumentException("There is already a piece at this position");
        }

        boolean validMove = false;
        for (Direction direction : Direction.values()) {
            if (willFlipPieces(piece, x, y, direction)) {
                validMove = true;
                break; //If we have found a valid direction, we can break the cycle
            }
        }

        if (validMove) {
            this.saveState(); // We save the state only if the movement is valid
            for (Direction direction : Direction.values()) {
                if (willFlipPieces(piece, x, y, direction)) {
                    flipPiecesInDirection(piece, x, y, direction);
                }
            }
            board[x][y] = piece; // We add the Piece after having shot the other Pieces
        } else {
            throw new IllegalArgumentException("Invalid move " + x + " " + y);
        }
    }


    /**
     * count the number of pieces of the specified color on the board
     *
     * @param color the color of the player
     * @return the number of pieces of the specified color on the board
     */
    public int countPieces(Color color) {
        int count = 0;
        for (Piece[] row : board) {
            for (Piece piece : row) {
                if (piece != null && piece.getColor() == color) {
                    count++;
                }
            }
        }
        return count;
    }

    /**
     * remove the piece from the board at the specified coordinates
     *
     * @param x x coordinate
     * @param y y coordinate
     */
    public void removePiece(int x, int y) {
        board[x][y] = null;
    }

    /**
     * verify if the player white  has any possible moves
     *
     * @return an ArrayList of possible moves
     */
    public ArrayList<Pair<Integer, Integer>> whitePossibleMovesList() {
        Piece piece = new Piece(Color.WHITE);
        ArrayList<Pair<Integer, Integer>> possibleMoves = new ArrayList<Pair<Integer, Integer>>();
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                try {
                    if (board[i][j] == null) {
                        for (Direction direction : Direction.values()) {
                            if (willFlipPieces(piece, i, j, direction)) {
                                possibleMoves.add(new Pair<Integer, Integer>(i, j));
                                break;
                            }
                        }
                    }
                } catch (Exception e) {
                    // Handle the exception gracefully, perhaps by logging it
                    e.printStackTrace();
                }
            }
        }
        return possibleMoves;
    }

    /**
     * verify if the player black has any possible moves
     *
     * @return an ArrayList of possible moves
     */
    public ArrayList<Pair<Integer, Integer>> blackPossibleMovesList() {
        Piece piece = new Piece(Color.BLACK);
        ArrayList<Pair<Integer, Integer>> possibleMoves = new ArrayList<Pair<Integer, Integer>>();
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                try {
                    if (board[i][j] == null) {
                        for (Direction direction : Direction.values()) {
                            if (willFlipPieces(piece, i, j, direction)) {
                                possibleMoves.add(new Pair<Integer, Integer>(i, j));
                                break;
                            }
                        }
                    }
                } catch (Exception e) {
                    // Handle the exception gracefully, perhaps by logging it
                    e.printStackTrace();
                }
            }
        }
        return possibleMoves;
    }

    /**
     * deep copy of the board
     *
     * @return a deep copy of the board
     */
    public Piece[][] deepCopy() {
        final Piece[][] result = new Piece[board.length][];
        for (int i = 0; i < board.length; i++) {
            result[i] = new Piece[board[i].length];
            for (int j = 0; j < board[i].length; j++) {
                if (board[i][j] != null) {
                    result[i][j] = new Piece(board[i][j].getColor());
                }
            }
        }
        return result;
    }

    /**
     * save the state of the board to the undo stack to allow the player to undo the last move
     * made during the game and keep track of the moves made during the game to allow the player to redo the moves
     * made during the game
     */
    public void saveState() {


        undoStack.push(deepCopy());


    }

    /**
     * undo the last move made by the player and update the board
     * to the previous state before the move was made
     */
    public void undoMove() {
        if (!undoStack.isEmpty()) {
            redoStack.push(deepCopy());
            this.board = undoStack.pop();


        }
    }

    /**
     * redo the last move made by the player and update the board
     * to the state before the undo was made
     */
    public void redoMove() {
        if (!redoStack.isEmpty()) {
            undoStack.push(deepCopy());
            this.board = redoStack.pop();


        }
    }

    /**
     * restart the game by clearing the board and adding the initial pieces to the board
     * and clearing the undo and redo stacks
     */
    public void restart() {
        clear();
        board[height / 2][width / 2] = new Piece(Color.WHITE);
        board[height / 2 - 1][width / 2 - 1] = new Piece(Color.WHITE);
        board[height / 2 - 1][width / 2] = new Piece(Color.BLACK);
        board[height / 2][width / 2 - 1] = new Piece(Color.BLACK);
        undoStack.clear();
        redoStack.clear();
    }


}
