#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld &world, int imageId, double startX, double startY,
          int startDirection, int depth, double size);
    virtual void doSomething(){};
    virtual void bonk(Actor *other) {}

    virtual bool isPeach() { return false; }
    virtual bool passable() = 0;
    virtual bool damageable() = 0;

    bool isAlive() { return alive; }

protected:
    StudentWorld &getWorld() { return world; }
    void die() { alive = false; }
    bool attemptMove(double dx, double dy, bool bonk = false);
    virtual bool testPosition(double x, double y, bool bonk);

    // Utility member functions
    bool overlappingWithPeach();

private:
    bool alive;
    StudentWorld &world;
};

class Peach : public Actor {
public:
    Peach(StudentWorld &world, double startX, double startY);
    void doSomething() override;
    void bonk(Actor *other) override;

    static const int JUMP = 0b0001,
                     FIRE = 0b0010,
                     STAR = 0b0100;

    bool isPeach() override { return true; }
    bool passable() override { return true; }
    bool damageable() override { return true; }

    void setHp(int hp) { this->hp = hp; }
    void addPower(int power) { powers |= power; }
    int getPowers() { return powers; }

private:
    int hp;
    int powers;
    int jumpDistance;
    bool grounded;
};

class Flag : public Actor {
public:
    Flag(StudentWorld &world, double startX, double startY, int imageId = IID_FLAG);

    void doSomething() override;

    virtual void gameSignal();

    bool passable() override { return true; }
    bool damageable() override { return false; }
};

class Mario : public Flag {
public:
    Mario(StudentWorld &world, double startX, double startY);

    void gameSignal() override;
};

//==============================================================================
// Powerups

class Powerup : public Actor {
public:
    Powerup(StudentWorld &world, int imageId, double startX, double startY);

    void doSomething();

    bool passable() override { return true; }
    bool damageable() override { return false; }

protected:
    virtual int points() = 0;
    virtual int power() = 0;
};

class Flower : public Powerup {
public:
    Flower(StudentWorld &world, double startX, double startY);

    static void create(StudentWorld &world, double startX, double startY);

private:
    int points() { return 50; }
    int power() { return Peach::FIRE; }
};

class Mushroom : public Powerup {
public:
    Mushroom(StudentWorld &world, double startX, double startY);

    static void create(StudentWorld &world, double startX, double startY);

private:
    int points() { return 75; }
    int power() { return Peach::JUMP; }
};

class Star : public Powerup {
public:
    Star(StudentWorld &world, double startX, double startY);

    static void create(StudentWorld &world, double startX, double startY);

private:
    int points() { return 100; }
    int power() { return Peach::STAR; }
};

//==============================================================================
// Enemies

class Enemy : public Actor {
public:
    Enemy(StudentWorld &world, double startX, double startY, int imageId);

    void bonk(Actor *other) override;
    void takeDamage();

    bool passable() override { return true; }
    bool damageable() override { return true; }

    double getMinX() { return minX; }
    double getMaxX() { return maxX; }
    void setMinX(double x) { minX = x; }
    void setMaxX(double x) { maxX = x; }

private:
    double minX, maxX;
};

class Goomba : public Enemy {
public:
    Goomba(StudentWorld &world, double startX, double startY, int imageId = IID_GOOMBA);

    bool testPosition(double x, double y, bool bonk) override;
    void doSomething() override;
};

class Koopa : public Goomba {
public:
    Koopa(StudentWorld &world, double startX, double startY);
};

//==============================================================================
// Obstacles

class Pipe : public Actor {
public:
    Pipe(StudentWorld &world, double startX, double startY, int imageId = IID_PIPE);

    bool passable() override { return false; }
    bool damageable() override { return false; }
};

class Block : public Pipe {
public:
    Block(StudentWorld &world, double startX, double startY);

    void bonk(Actor *other) override;
    void setPowerup(void (*creator)(StudentWorld &, double, double));

private:
    void (*createPowerup)(StudentWorld &, double, double);
};

//==============================================================================
// Utility functions

bool areColliding(double xStart1, double yStart1, double xStart2, double yStart2);

#endif // ACTOR_H_
