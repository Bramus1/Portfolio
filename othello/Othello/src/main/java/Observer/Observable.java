package Observer;

import java.util.ArrayList;

/**
 * Observable class is used to notify the observers when a change is made.
 */
public class Observable {
    private ArrayList<Observer> observer;

    public Observable() {
        observer = new ArrayList<>();
    }

    /**
     * Add observer to the list of observers
     *
     * @param observer the observer to be added
     */
    public void addObserver(Observer observer) {
        this.observer.add(observer);
    }

    /**
     * Remove observer from the list of observers
     *
     * @param obj the observer to be removed
     */
    public void notifyObserver(Object obj) {
        for (Observer o : observer) {
            o.update(obj);
        }
    }


}
