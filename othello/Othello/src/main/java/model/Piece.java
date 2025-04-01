package model;

import java.util.Objects;

/**
 * Piece class is used to represent a piece on the board.
 */
public class Piece {
    private Color color;

    /**
     * Constructor for Piece class that takes a color as a parameter.
     *
     * @param color the color of the piece
     */
    public Piece(Color color) {
        Objects.requireNonNull(color, "you need a color");

        this.color = color;
    }

    /**
     * Method to get the color of the piece.
     *
     * @return the color of the piece
     */
    public Color getColor() {
        return color;
    }

    /**
     * Method to set the color of the piece.
     *
     * @param color the color of the piece
     */
    public void setColor(Color color) {
        this.color = color;
    }

    /**
     * Method to get the opposite color of the piece.
     * If the color is white, it returns black and vice versa.
     *
     * @return the opposite color of the piece
     */
    public Color oppositeColor() {
        if (color == Color.WHITE) {
            return Color.BLACK;
        } else {
            return Color.WHITE;
        }
    }

    /**
     * Method to return the string representation of the piece.
     *
     * @return the string representation of the piece
     */
    public String toString() {
        if (color == Color.WHITE) {
            return "W ";
        } else {
            return "B ";
        }
    }

}
