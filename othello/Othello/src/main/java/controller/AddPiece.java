package controller;

import model.Color;
import model.Game;

import java.util.Objects;

/**
 * AddPiece class implements the Commands interface and is used to add a piece to the board.
 */
public class AddPiece implements Commands {

    private int x;
    private int y;
    private Color color;
    private Game game;

    /**
     * Constructor for AddPiece
     *
     * @param x    the x coordinate of the piece
     * @param y    the y coordinate of the piece
     * @param game the game object
     */
    public AddPiece(int x, int y, Game game) {
        Objects.requireNonNull(game, "you need a game");
        this.game = game;
        this.x = x;
        this.y = y;
        this.color = game.getCurrentPlayer().getColor();


    }

    /**
     * Adds a piece to the board at the specified coordinates x and y and updates the game state.
     *
     * @throws Exception
     */
    @Override
    public void execute() throws Exception {

        game.addPiece(x, y);

    }

    /**
     * Removes the piece from the board at the specified coordinates x and y and updates the game state.
     */
    @Override
    public void undo() {
        game.undo();
    }

    /**
     * Adds the piece back to the board at the specified coordinates x and y and updates the game state.
     */
    @Override
    public void redo() {
        game.redo();
    }
}