#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Actor;

struct BonkProps {
    bool left, bot, top, right;
};

struct Surroundings {
    struct Pair {
        double space;
        Actor *actor;
    };

    Pair &topLeft, &topMid, &topRight,
        &midLeft, &midRight,
        &botLeft, &botMid, &botRight;
    Pair data[3][3] = {{{-1, nullptr}, {-1, nullptr}, {-1, nullptr}},
                       {{-1, nullptr}, {-1, nullptr}, {-1, nullptr}},
                       {{-1, nullptr}, {-1, nullptr}, {-1, nullptr}}};
    Surroundings()
        : topLeft(data[0][0]), topMid(data[0][1]), topRight(data[0][2]),
          midLeft(data[1][0]), midRight(data[1][2]),
          botLeft(data[2][0]), botMid(data[2][1]), botRight(data[2][2]) {}
    void clear() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                data[i][j] = {-1, nullptr};
            }
        }
    }
    void print() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                std::cerr << data[i][j].space << '\t';
            }
            std::cerr << std::endl;
        }
    }
};

class Actor : public GraphObject {
public:
    Actor(StudentWorld &world, int imageId, double startX, double startY,
          int startDirection, double size, int depth);
    virtual void doSomething() = 0;
    virtual void bonk(Actor *other, BonkProps props) {}

    virtual bool passable() = 0;
    virtual bool movable() = 0;

    void applySpaceProps(Surroundings newProps);

protected:
    StudentWorld &getWorld() { return world; }
    Surroundings spaceProps;

private:
    StudentWorld &world;
};

class Movable : public virtual Actor {
public:
    Movable() {}
    virtual bool movable() { return true; };

protected:
    bool attemptMove(double dx, double dy);

    bool checkSpace();

protected:
    double dx, dy;
};

class Peach : public Movable {
public:
    Peach(StudentWorld &world, double startX, double startY);
    void doSomething();
    void bonk(Actor *other, BonkProps props);

    bool passable() { return true; }

    void commitBonk();

private:
    int hp = 1;
    int powers = 0;
    int jumpDist = 0;
};

class Block : public Actor {
public:
    Block(StudentWorld &world, double startX, double startY);
    void doSomething();

    // Property methods
    bool passable() { return false; }
    bool movable() { return false; }
};

class Pipe : public Actor {
public:
    Pipe(StudentWorld &world, double startX, double startY);
    void doSomething();

    // Property methods
    bool passable() { return false; }
    bool movable() { return false; }
};

Surroundings calcSpace(Actor *actor1, Actor *actor2);

#endif // ACTOR_H_
