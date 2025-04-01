package view.javaFX;

import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import model.Board;
import model.Game;
import strategy.CleverStrategy;
import strategy.RandomStrategy;
import strategy.Strategy;

public class SettingsGame {

    private MainPane mainPane;
    private Game game;

    public SettingsGame(MainPane mainPane) {
        this.mainPane = mainPane;
        game = mainPane.getGame();
    }

    private Stage stage;
    Label sizeLabel = new Label("Enter the size of the board:");
    TextField sizeTextField = new TextField();

    public void changeScene(Stage primaryStage) throws Exception {
        stage = primaryStage;
        primaryStage.setTitle("Othello Game settings page");

        sizeLabel = new Label("Enter the size of the board:");
        sizeTextField = new TextField();
        Label modeLabel = new Label("Choose the game mode:");
        ComboBox<String> modeComboBox = new ComboBox<>();
        modeComboBox.getItems().addAll("Player vs Strategy", "Two Players");

        Label strategyLabel = new Label("Choose the strategy:");
        ComboBox<String> strategyComboBox = new ComboBox<>();
        strategyComboBox.getItems().addAll("Random Strategy", "Clever Strategy");

        Button startButton = new Button("Start Game");

        VBox vbox = new VBox(10);
        vbox.setPadding(new Insets(10));
        vbox.getChildren().addAll(sizeLabel, sizeTextField, modeLabel, modeComboBox, strategyLabel, strategyComboBox, startButton);

        BorderPane borderPane = new BorderPane();
        borderPane.setCenter(vbox);

        Scene scene = new Scene(borderPane, 600, 250);
        primaryStage.setScene(scene);

        primaryStage.show();

        startButton.setOnAction(e -> {
            String selectedMode = modeComboBox.getValue();
            String selectedStrategy = strategyComboBox.getValue();

            if (selectedMode != null && selectedMode.equals("Player vs Strategy") && selectedStrategy == null) {
                // Shows an informational dialog
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setTitle("Information Dialog");
                alert.setHeaderText(null);
                alert.setContentText("Please select a strategy before starting the game.");
                alert.showAndWait();
            } else if (selectedMode == null) {
                // Shows an informational dialog
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setTitle("Information Dialog");
                alert.setHeaderText(null);
                alert.setContentText("Please select a mode before starting the game.");
                alert.showAndWait();
            } else {
                // start the game only if there are no probles with the input
                try {
                    startGame(sizeTextField.getText(), selectedMode, selectedStrategy);
                } catch (Exception ex) {
                    throw new RuntimeException(ex);
                }
            }
        });

    }


    private void startGame(String sizeInput, String mode, String strategy) throws Exception {
        try { // I check if the input is a valid number
            int size = Integer.parseInt(sizeInput);
            // I check if the size is valid
            if (size < 3 || size > 15) {
                showErrorAlert("Invalid Size", "The size must be between 3 and 15.");
            }// if the size is valid, I start the game
            else {
                // I create the game based on the mode selected (player vs strategy)
                if (mode.equals("Player vs Strategy")) {

                    Strategy selectedStrategy = getStrategy(strategy);
                    Board board = new Board(size, size);

                    game = new Game(board);

                    game.setStrategy(selectedStrategy);
                    mainPane.setGame(game);
                    ControllerFx controller = new ControllerFx(game, mainPane);
                    controller.initialise(controller);

                } // I create the game based on the mode selected (player vs player)
                else {
                    Board board = new Board(size, size);

                    game = new Game(board);
                    mainPane.setGame(game);
                    ControllerFx controller = new ControllerFx(game, mainPane);
                    controller.initialise(controller);

                }

                mainPane.init(); // I initialise the main pane
            }
        } catch (NumberFormatException e) {
            showErrorAlert("Invalid Input", "Please enter a valid number.");
        }
    }

    /**
     * Method to get the strategy.
     *
     * @param strategyName the strategy chosen
     * @return the strategy chosen
     */
    private Strategy getStrategy(String strategyName) {
        switch (strategyName) {
            case "Random Strategy":
                return new RandomStrategy();
            case "Clever Strategy":
                return new CleverStrategy();
            default:
                return new RandomStrategy();
        }
    }

    /**
     * Method to show an error alert.
     *
     * @param title   the title of the alert
     * @param message the message of the alert
     */
    private void showErrorAlert(String title, String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }


}
