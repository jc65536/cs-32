#include <iostream>

#include "CarMap.h"

CarMap::CarMap() {}

double CarMap::miles(std::string license) const {
    double mi = -1;
    fleet.get(license, mi);
    return mi;
}

int CarMap::fleetSize() const {
    return fleet.size();
}

void CarMap::print() const {
    std::string license;
    double mi;
    for (int i = 0; i < fleetSize(); i++) {
        fleet.get(i, license, mi);
        std::cout << license << " " << mi << std::endl;
    }
}

bool CarMap::addCar(std::string license) {
    return fleet.insert(license, 0);
}

bool CarMap::drive(std::string license, double distance) {
    if (distance < 0)
        return false;
    return fleet.update(license, miles(license) + distance);
}
