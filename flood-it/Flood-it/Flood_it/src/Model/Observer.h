#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
public:
    /**
     * @brief update function to update the observer
     */
    virtual void update() = 0;
};

#endif // OBSERVER_H
