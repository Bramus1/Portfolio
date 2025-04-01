package Observer;

/**
 * Observer interface is used to update the observers when a change is made.
 */
public interface Observer {
    /**
     * Update the observer
     *
     * @param obj the object to be updated
     */
    void update(Object obj);
}
