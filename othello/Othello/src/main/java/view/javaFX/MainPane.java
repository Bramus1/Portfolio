package view.javaFX;

import Observer.Observer;
import controller.AddPiece;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Point2D;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.input.MouseButton;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import model.Game;

/**
 * MainPane class is used to represent the main pane of the game.
 * it contains the menu bar and the data pane
 */
public class MainPane extends VBox implements Observer {

    private Game game;
    private MenuB menu;
    private DataPane dataPane;
    private Stage stage;
    private EventHandler<ActionEvent> undoHandler;
    private EventHandler<ActionEvent> redoHandler;
    private EventHandler<javafx.scene.input.MouseEvent> clickMouseHandler;
    private EventHandler<ActionEvent> surrenderHandler;

    /**
     * Constructor for the MainPane class.
     *
     * @param stage the stage of the game
     * @param game  the game to be rendered
     */
    public MainPane(Stage stage, Game game) {

        this.game = game;
        this.stage = stage;
        dataPane = new DataPane();
        menu = new MenuB();
        menu.minWidthProperty().bind(this.widthProperty());
        menu.maxWidthProperty().bind(this.widthProperty());

        this.renderBoard(game);
        dataPane.getBoardPane().highlightPossibleMoves(game);


        this.getChildren().addAll(menu, dataPane);
        this.setAlignment(Pos.CENTER);

        this.setSpacing(10);

    }

    /**
     * Method to render the board with colored cells and pieces.
     *
     * @param game the game to be rendered
     */
    private void renderBoard(Game game) {
        dataPane.renderBoard(game);
    }

    /**
     * Method to initialize the main pane.
     */
    public void init() {
        MainPane mainPane = this;
        this.renderBoard(game);

        dataPane.setPlayerBlackScore(game);
        dataPane.setPlayerWhiteScore(game);
        Scene scene = new Scene(mainPane);
        dataPane.getBoardPane().highlightPossibleMoves(game);
        stage.setScene(scene);
        stage.setTitle("Othello");
        stage.show();
        game.notifyObserver(game);
    }

    /**
     * Method to initialize the game
     *
     * @param controller the controller of the game
     */
    public void initialise(ControllerFx controller) {


        // here I create the event handlers for the undo, redo, clickMouse and surrender buttons

        undoHandler = new EventHandler<ActionEvent>() {
            @Override
            // here I handle the undo event
            public void handle(ActionEvent event) {

                //if the player is playing vs strategy I call the undo strategy
                if (game.getStrategy() != null) {
                    try {
                        controller.undo();
                        controller.undoStrategy();

                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                } //else I call the undo method if I play vs player
                else {
                    try {
                        controller.undo();
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        };

        redoHandler = new EventHandler<ActionEvent>() {
            @Override
            // here I handle the redo event
            public void handle(ActionEvent event) {
                //if the player is playing vs strategy I call the undo strategy
                if (game.getStrategy() != null) {
                    try {
                        controller.redo();
                        controller.redoStrategy();

                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                } //else I call the undo method if I play vs player
                else {
                    try {
                        controller.redo();
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        };


        clickMouseHandler = new EventHandler<javafx.scene.input.MouseEvent>() {

            @Override
            // here I handle the mouse click event
            public void handle(javafx.scene.input.MouseEvent mouseEvent) {

                if (mouseEvent.getButton() == MouseButton.PRIMARY) {
                    // if current player has moves
                    if (game.hasMoves(game.getCurrentPlayer())) {


                        double mouseX = mouseEvent.getSceneX();
                        double mouseY = mouseEvent.getSceneY();

                        // Convert mouse coordinates from scene coordinates to local BoardPane coordinates
                        double localX = dataPane.getBoardPane().sceneToLocal(new Point2D(mouseX, mouseY)).getX();
                        double localY = dataPane.getBoardPane().sceneToLocal(new Point2D(mouseX, mouseY)).getY();


                        // Add the corresponding offset to the edge or margin of the BoardPane
                        double offsetXBoard = dataPane.getBoardPane().getInsets().getLeft();
                        double offsetYBoard = dataPane.getBoardPane().getInsets().getTop();
                        localX -= offsetXBoard;
                        localY -= offsetYBoard;


                        // Add the corresponding offset to the InfoPane padding
                        double offsetXInfoPane = dataPane.getInfoPane().getPadding().getLeft();
                        double offsetYInfoPane = dataPane.getInfoPane().getPadding().getTop();
                        localX -= offsetXInfoPane;
                        localY -= offsetYInfoPane;
                        // Calculate the actual cell size, considering margin and padding
                        double cellWidth = dataPane.getBoardPane().getCellWidth() + dataPane.getBoardPane().getHgap();
                        double cellHeight = dataPane.getBoardPane().getCellHeight() + dataPane.getBoardPane().getVgap();


                        // Calculates the position on the boardPane based on the local coordinates of the mouse click
                        int row = (int) ((localY + dataPane.getBoardPane().getVgap()) / cellHeight);
                        int column = (int) ((localX + dataPane.getBoardPane().getHgap()) / cellWidth);


                        try {
                            // try to add the piece
                            AddPiece addPiece = new AddPiece(row, column, game);
                            controller.addPieceCommand(addPiece);
                        } catch (Exception e) {
                            // here I catch the error  with an alert
                            Alert alert = new Alert(Alert.AlertType.WARNING, "Invalid move, try again", ButtonType.OK);
                            alert.showAndWait();
                            if (alert.getResult() == ButtonType.OK) {
                                alert.close();
                            }
                            e.printStackTrace(); // Tratta l'eccezione in modo appropriato
                        }
                        //here I check if player is playing vs strategy if yes I call the strategy
                        try {
                            controller.playVsStrategy();
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                    // if the player has no moves
                    else {

                        Alert alert = new Alert(Alert.AlertType.WARNING, "No more moves for you " + game.getCurrentPlayer().toString(), ButtonType.OK);
                        alert.showAndWait();
                        if (alert.getResult() == ButtonType.OK) {
                            alert.close();
                        }
                        //I change player and call the strategy
                        // and if stategy is available I call it
                        game.changePlayer();
                        try {
                            controller.playVsStrategy();
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }

                        game.notifyObserver(game);
                    }

                game.notifyObserver(game);

            }
                }


        };

        surrenderHandler = new EventHandler<ActionEvent>() {
            @Override
            // here I handle the surrender event
            public void handle(ActionEvent event) {
                try {


                    Alert alert = new Alert(Alert.AlertType.WARNING, "Player "
                            + game.getCurrentPlayer()
                            + " you surrendered  \n"
                            + game.getCurrentPlayer().getOppoColor()
                            + " you win ", ButtonType.OK);
                    alert.showAndWait();
                    if (alert.getResult() == ButtonType.OK) {
                        alert.close();
                    }


                    controller.surrender(); // here I call the surrender method
                    game.restart(); // here I restart the game with the notify observer


                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }

        };


        dataPane.getInfoPane().getUndoButton().setOnAction(undoHandler); // here I set the event handler for the undo button
        dataPane.getInfoPane().getRedoButton().setOnAction(redoHandler); // here I set the event handler for the redo button
        dataPane.getBoardPane().setOnMouseClicked(clickMouseHandler); // here I set the event handler for the mouse click
        dataPane.getInfoPane().getSurrenderButton().setOnAction(surrenderHandler); // here I set the event handler for the surrender button


    }

    /**
     * Method to update the object.From the observer
     *
     * @param obj the object to be updated
     */
    @Override
    public void update(Object obj) {
        Game game = (Game) obj;
        dataPane.updateBoard(game);
        if (game.isGameOver()) {
            dataPane.updateBoard(game);

            displayWinner(game);
            game.restart(); // this method has a notify observer


        }

    }

    /**
     * Method to get the game.
     *
     * @return the game
     */
    public Game getGame() {

        return game;
    }

    /**
     * Method to set the game.
     *
     * @param game the game to be set
     */
    public void setGame(Game game) {
        this.game = game;
    }

    /**
     * Method to display the winner of the game.
     *
     * @param game the game to be displayed
     */
    public void displayWinner(Game game) {
        if (game.getPlayerBlack().getScore() > game.getPlayerWhite().getScore()) {
            Alert alert = new Alert(Alert.AlertType.INFORMATION, "Player Black you Win " + game.getPlayerBlack().getScore(), ButtonType.OK);
            alert.showAndWait();
            if (alert.getResult() == ButtonType.OK) {
                alert.close();
            }


        } else if (game.getPlayerBlack().getScore() < game.getPlayerWhite().getScore()) {
            Alert alert = new Alert(Alert.AlertType.INFORMATION, "Player White you Win " + game.getPlayerWhite().getScore(), ButtonType.OK);
            alert.showAndWait();
            if (alert.getResult() == ButtonType.OK) {
                alert.close();
            }
        } else {

            Alert alert = new Alert(Alert.AlertType.INFORMATION, "It's a Draw", ButtonType.OK);
            alert.showAndWait();
            if (alert.getResult() == ButtonType.OK) {
                alert.close();
            }


        }


    }


}
