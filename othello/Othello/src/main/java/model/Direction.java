package model;

/**
 * Direction enum is used to represent the direction of the pieces.
 */
public enum Direction {
    UP(0, -1),

    DOWN(0, 1),

    LEFT(-1, 0),

    RIGHT(1, 0),

    UPLEFT(-1, -1),
    UPRIGHT(1, -1),
    DOWNLEFT(-1, 1),
    DOWNRIGHT(1, 1);


    private int x;
    private int y;

    /**
     * Constructor for the Direction enum.
     *
     * @param x the x coordinate
     * @param y the y coordinate
     */

    Direction(int x, int y) {

        this.x += x;
        this.y += y;
    }

}
