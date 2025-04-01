#include "Observable.h"

void Observable::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Observable::removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Observable::notifyObservers() {
    for (Observer* observer : observers) {
        observer->update();
    }
}
