package controller;

/**
 * Commands interface is used to implement the execute, undo and redo methods.
 */
public interface Commands {
    /**
     * Executes the command.
     *
     * @throws Exception
     */
    void execute() throws Exception;

    /**
     * Undoes the command.
     */
    void undo();

    /**
     * Redoes the command.
     *
     * @throws Exception
     */
    void redo() throws Exception;


}
