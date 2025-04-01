package view.javaFX;

import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.geometry.Pos;
import model.Color;
import model.Game;

public class InfoPane extends GridPane {
    private Label player1Label;
    private Label player2Label;
    private Label player1Score;
    private Label player2Score;
    private Button undoButton;
    private Button redoButton;
    private Label currentPlayerLabel;
    private Button surrenderLabel;


    public InfoPane() {
        setAlignment(Pos.TOP_RIGHT);
        undoButton = new Button("Undo");
        redoButton = new Button("Redo");
        player1Label = new Label("Player Black : ");
        player2Label = new Label("Player White : ");
        player1Score = new Label("0");
        player2Score = new Label("0");
        currentPlayerLabel = new Label("Current player : ");
        surrenderLabel = new Button("Surrender");


        this.add(player1Label, 0, 0);
        this.add(player1Score, 1, 0);
        this.add(player2Label, 0, 1);
        this.add(player2Score, 1, 1);
        this.add(undoButton, 0, 2);
        this.add(redoButton, 1, 2);
        this.add(currentPlayerLabel, 0, 3);
        this.add(surrenderLabel, 0, 4);
        this.setAlignment(Pos.CENTER);
    }

    public Button getUndoButton() {
        return undoButton;
    }

    public Button getRedoButton() {
        return redoButton;
    }

    public void setPlayerBlackScore(Game game) {
        player1Score.setText(game.getBoard().countPieces(Color.BLACK) + "");

    }

    public void setPlayerWhiteScore(Game game) {
        player2Score.setText(game.getBoard().countPieces(Color.WHITE) + "");

    }

    public void updateInfo(Game game) {
        setPlayerBlackScore(game);
        setPlayerWhiteScore(game);
        setCurrentPlayerLabel(game);
    }

    public void setCurrentPlayerLabel(Game game) {
        currentPlayerLabel.setText("Player turn : " + game.getCurrentPlayer().toString());
    }

    public Button getSurrenderButton() {
        return surrenderLabel;
    }
}
