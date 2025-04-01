#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include "Observer.h"
/**
 * @brief The Observable class
 */
class Observable {
private:
    std::vector<Observer*> observers;

public:
    /**
     * @brief addObserver add an observer
     * @param observer
     */
    void addObserver(Observer* observer);
    /**
     * @brief removeObserver remove an observer
     * @param observer
     */
    void removeObserver(Observer* observer);
    /**
     * @brief notifyObservers notify the observers
     */
    void notifyObservers();
};

#endif // OBSERVABLE_H
