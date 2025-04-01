package model;

import java.util.Objects;

/**
 * Player class is used to represent a player in the game.
 */
public class Player {
    private Color color;

    private int score;

    /**
     * Constructor for Player class that takes a color as a parameter.
     *
     * @param color the color of the player
     */
    public Player(Color color) {

        Objects.requireNonNull(color, "you need a color");

        this.color = color;
        this.score = 0;

    }

    /**
     * Method to get the color of the player.
     *
     * @return the color of the player
     */
    public Color getColor() {
        return color;
    }

    /**
     * Method to get the score of the player.
     *
     * @return the score of the player
     */
    public int getScore() {
        return score;
    }

    /**
     * Method to set the score of the player.
     *
     * @param points
     */
    public void Score(int points) {
        this.score = points;
    }

    /**
     * Method to return the string representation of the player .
     *
     * @return the string representation of the player
     */
    @Override
    public String toString() {
        return this.color.toString();
    }

    public Color getOppoColor() {
        if (this.color == Color.BLACK) {
            return Color.WHITE;
        }
        return Color.BLACK;
    }
}
