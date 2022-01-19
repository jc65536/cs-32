#include <cassert>

#include "CarMap.h"

KeyType l(int i) {
    KeyType s = "aa";
    s[0] += i / 26;
    s[1] += i % 26;
    return s;
}

int main() {
    /*
    aa 0
    ab 1
    ac 2
    ad 3
    ae 4
    aa 0
    aa 0
    aa 2
    aa 2
    aa 2
    */

    CarMap m;

    // double miles(std::string license) const;
    m = CarMap();

    // Licence doesn't exist
    assert(m.miles(l(0)) == -1);

    // License exists
    m.addCar(l(0));
    assert(m.miles(l(0)) == 0);

    // Miles > 0
    m.drive(l(0), 2);
    assert(m.miles(l(0)) == 2);

    // int fleetSize() const;
    m = CarMap();

    // Size 0
    assert(m.fleetSize() == 0);

    // Some size
    for (int i = 0; i < 5; i++)
        m.addCar(l(i));
    assert(m.fleetSize() == 5);

    // Max size
    for (int i = 0; i < 2 * DEFAULT_MAX_ITEMS; i++)
        m.addCar(l(i));
    assert(m.fleetSize() == DEFAULT_MAX_ITEMS);

    // void print() const;
    m = CarMap();
    for (int i = 0; i < 5; i++) {
        m.addCar(l(i));
        m.drive(l(i), i);
    }
    m.print();
    /*
    aa 0
    ab 1
    ac 2
    ad 3
    ae 4
    */

    // bool addCar(std::string license);
    m = CarMap();

    // Able to add
    assert(m.addCar(l(0)));
    m.print(); // aa 0

    // Can't add due to conflict
    assert(!m.addCar(l(0)));
    m.print(); // aa 0

    // Can't add due to size
    for (int i = 1; i < DEFAULT_MAX_ITEMS; i++)
        m.addCar(l(i));
    assert(!m.addCar(l(DEFAULT_MAX_ITEMS + 1)));

    // bool drive(std::string license, double distance);
    m = CarMap();
    m.addCar(l(0));

    // Can drive
    assert(m.drive(l(0), 2));
    m.print(); // aa 2

    // License doesn't exist
    assert(!m.drive(l(1), 2));
    m.print(); // aa 2

    // Distance < 0
    assert(!m.drive(l(0), -2));
    m.print(); // aa 2
}
