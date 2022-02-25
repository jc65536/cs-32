#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Actor;
class Movable;

struct BonkProps {
    bool left, bot, top, right;
};

class Actor : public GraphObject {
public:
    Actor(StudentWorld &world, int imageId, double startX, double startY,
          int startDirection, double size, int depth);
    virtual void doSomething(){};
    virtual void bonk(Actor *other, BonkProps props) {}

    virtual bool passable() = 0;
    virtual Movable *movable() = 0;

    void print();

protected:
    StudentWorld &getWorld() { return world; }

private:
    StudentWorld &world;
};

class Movable : public virtual Actor {
public:
    Movable() {}
    virtual Movable *movable() { return this; };

    void addNearbyBlock(Actor *actor);

protected:
    void startMove();
    bool attemptMove(double ddx, double ddy);
    void commitMove();

private:
    double dx, dy;
    std::vector<Actor *> nearbyBlocks;
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
    bool grounded = true;
};

class Block : public Actor {
public:
    Block(StudentWorld &world, double startX, double startY, int imageId = IID_BLOCK);

    void bonk(Actor *other, BonkProps props);

    // Property methods
    bool passable() { return false; }
    Movable *movable() { return nullptr; }
};

class Pipe : public Block {
public:
    Pipe(StudentWorld &world, double startX, double startY);
};

//==============================================================================
// Utility functions

inline double distance2(double x, double y) {
    return x * x + y * y;
}

inline bool areNearby(Actor *actor1, Actor *actor2) {
    return distance2(actor1->getX() - actor2->getX(), actor1->getY() - actor2->getY()) < 512;
}

bool areColliding(double x1, double y1, double x2, double y2,
                  BonkProps *props1 = nullptr, BonkProps *props2 = nullptr);

#endif // ACTOR_H_
