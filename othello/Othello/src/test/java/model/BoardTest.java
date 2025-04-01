package model;

import javafx.util.Pair;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class BoardTest {

    private Board board;

    @BeforeEach
    void setUp() {
        board = new Board(8, 8);
    }

    @Test
    void getPiece() {
        Piece piece = board.getPiece(3, 4);
        assertEquals(Color.BLACK, piece.getColor());
    }

    @Test
    void getHeight() {
        assertEquals(8, board.getHeight());
    }

    @Test
    void getWidth() {
        assertEquals(8, board.getWidth());
    }

    @Test
    void getBoard() {
        Piece[][] boardState = board.getBoard();
        assertEquals(8, boardState.length);
        assertEquals(8, boardState[0].length);
    }


    @Test
    void clear() {
        board.clear();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                assertNull(board.getPiece(i, j));
            }
        }
    }

    @Test
    void verifyUp() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void verifyDown() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void verifyLeft() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void verifyRight() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void verifyUpLeft() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void verifyUpRight() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void verifyDownLeft() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void verifyDownRight() {
        assertFalse(board.verifyUp(3, 4, Color.BLACK));
        assertFalse(board.verifyUp(3, 4, Color.WHITE));
    }

    @Test
    void addPiece() {
        assertEquals(Color.BLACK, board.getPiece(3, 4).getColor());
    }

    @Test
    void countPieces() {
        assertEquals(2, board.countPieces(Color.BLACK));
        assertEquals(2, board.countPieces(Color.WHITE));
    }

    @Test
    void removePiece() {
        board.removePiece(3, 4);
        assertNull(board.getPiece(3, 4));
    }

    @Test
    void whitePossibleMovesList() {
        List<Pair<Integer, Integer>> possibleMoves = board.whitePossibleMovesList();
        assertEquals(4, possibleMoves.size());
    }

    @Test
    void blackPossibleMovesList() {
        List<Pair<Integer, Integer>> possibleMoves = board.blackPossibleMovesList();
        assertEquals(4, possibleMoves.size());
    }



}