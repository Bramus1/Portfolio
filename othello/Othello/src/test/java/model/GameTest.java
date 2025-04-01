package model;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class GameTest {

    private Game game;
    private Board board;

    @BeforeEach
    void setUp() {
        board = new Board(10, 10);
        game = new Game(board);
    }

    @Test
    void getBoard() {
        assertEquals(board, game.getBoard());
    }

    @Test
    void changePlayer() {
        assertEquals(Color.BLACK, game.getCurrentPlayer().getColor());
        game.changePlayer();
        assertEquals(Color.WHITE, game.getCurrentPlayer().getColor());
        game.changePlayer();
        assertEquals(Color.BLACK, game.getCurrentPlayer().getColor());
    }

    @Test
    void getCurrentPlayer() {
        assertEquals(Color.BLACK, game.getCurrentPlayer().getColor());
    }

    @Test
    void addPiece() throws Exception {
        game.addPiece(3, 4);
        assertEquals(Color.BLACK, game.getBoard().getPiece(5, 4).getColor());
        assertEquals(4, game.getPlayerBlack().getScore());
    }

    @Test
    void isGameOver() {
        assertFalse(game.isGameOver());
        game.getBoard().clear();
        assertTrue(game.isGameOver());
    }

    @Test
    void scoreCurrentPlayer() {
        assertEquals("2", game.scoreCurrentPlayer());
        game.changePlayer();
        assertEquals("2", game.scoreCurrentPlayer());
    }

    @Test
    void removePiece()throws Exception {

        game.removePiece(3, 4);
        assertNull(game.getBoard().getPiece(3, 4));
    }

    @Test
    void setGameOver() {
        game.setGameOver(true);
        assertTrue(game.getGameOver());
    }

    @Test
    void hasMoves() {
        assertTrue(game.hasMoves(game.getCurrentPlayer()));
        game.getBoard().clear();
        assertFalse(game.hasMoves(game.getCurrentPlayer()));
    }

    @Test
    void getPlayerBlack() {
        assertEquals(Color.BLACK, game.getPlayerBlack().getColor());
    }

    @Test
    void getPlayerWhite() {
        assertEquals(Color.WHITE, game.getPlayerWhite().getColor());
    }

    @Test
    void getGameOver() {
        assertFalse(game.getGameOver());
    }
}