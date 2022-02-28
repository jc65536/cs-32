#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld &world, int imageId, double startX, double startY,
          int startDirection, int depth, double size);

    virtual void doSomething() {}
    virtual void bonk(Actor *other) {}
    virtual bool testPosition(double x, double y, bool bonk);
    virtual void takeDamage() {}

    bool isPeach();
    virtual bool passable() = 0;
    virtual bool damageable() = 0;
    bool isAlive() { return alive; }

protected:
    StudentWorld &getWorld() { return world; }
    void die() { alive = false; }
    bool attemptMove(double dx, double dy, bool bonk = false);
    bool collidingWithPeach();

private:
    bool alive;
    StudentWorld &world;
};

class Peach : public Actor {
public:
    Peach(StudentWorld &world, double startX, double startY);

    void doSomething() override;
    void bonk(Actor *other) override;
    void takeDamage() override;

    static const int JUMP = 0b0001,
                     FIRE = 0b0010,
                     STAR = 0b0100;

    void addPower(int power);
    bool hasPower(int power) { return powers & power; }

    bool passable() override { return true; }
    bool damageable() override { return true; }

private:
    int hp;
    int powers;
    int fireCountdown;
    int invincibilityCountdown;
    int starCountdown;
    int jumpDistance;
    bool grounded;
};

//==============================================================================
// Flags

class Flag : public Actor {
public:
    Flag(StudentWorld &world, double startX, double startY,
         int imageId = IID_FLAG);

    void doSomething() override;

    bool passable() override { return true; }
    bool damageable() override { return false; }

protected:
    virtual int status() { return GWSTATUS_FINISHED_LEVEL; }
};

class Mario : public Flag {
public:
    Mario(StudentWorld &world, double startX, double startY);

private:
    int status() { return GWSTATUS_PLAYER_WON; }
};

//==============================================================================
// Powerups

class Powerup : public Actor {
public:
    Powerup(StudentWorld &world, int imageId, double startX, double startY);

    void doSomething() override;

    bool passable() override { return true; }
    bool damageable() override { return false; }

protected:
    virtual int points() = 0;
    virtual int power() = 0;
};

class Flower : public Powerup {
public:
    Flower(StudentWorld &world, double startX, double startY);

private:
    int points() { return 50; }
    int power() { return Peach::FIRE; }
};

class Mushroom : public Powerup {
public:
    Mushroom(StudentWorld &world, double startX, double startY);

private:
    int points() { return 75; }
    int power() { return Peach::JUMP; }
};

class Star : public Powerup {
public:
    Star(StudentWorld &world, double startX, double startY);

private:
    int points() { return 100; }
    int power() { return Peach::STAR; }
};

//==============================================================================
// Enemies

class Enemy : public Actor {
public:
    Enemy(StudentWorld &world, int imageId, double startX, double startY);

    void bonk(Actor *other) override;

    virtual void takeDamage() override;

    double getMinX() { return minX; }
    double getMaxX() { return maxX; }
    void setMinX(double x) { minX = x; }
    void setMaxX(double x) { maxX = x; }

    bool passable() override { return true; }
    bool damageable() override { return true; }

private:
    double minX, maxX;
};

class Goomba : public Enemy {
public:
    Goomba(StudentWorld &world, double startX, double startY,
           int imageId = IID_GOOMBA);

    void doSomething() override;
    bool testPosition(double x, double y, bool bonk) override;
};

class Koopa : public Goomba {
public:
    Koopa(StudentWorld &world, double startX, double startY);

    void takeDamage() override;
};

class Piranha : public Enemy {
public:
    Piranha(StudentWorld &world, double startX, double startY);

    void doSomething() override;

private:
    int fireCountdown;
};

//==============================================================================
// Obstacles

class Pipe : public Actor {
public:
    Pipe(StudentWorld &world, double startX, double startY,
         int imageId = IID_PIPE);

    bool passable() override { return false; }
    bool damageable() override { return false; }
};

using SpawnFunction = void (*)(StudentWorld &, double, double);

class Block : public Pipe {
public:
    Block(StudentWorld &world, double startX, double startY, SpawnFunction spawn = nullptr);

    void bonk(Actor *other) override;

private:
    SpawnFunction spawnPowerup;
};

//==============================================================================
// Projectiles

class Projectile : public Actor {
public:
    Projectile(StudentWorld &world, int imageId, double startX, double startY,
               int startDirection);

    void doSomething() override;

    bool passable() override { return true; }
    bool damageable() override { return false; }

protected:
    virtual bool shouldDamage(Actor *actor) = 0;
};

class PeachFireball : public Projectile {
public:
    PeachFireball(StudentWorld &world, double startX, double startY,
                  int startDirection);

protected:
    bool shouldDamage(Actor *actor) override {
        return actor->damageable() && !actor->isPeach();
    }
};

class PiranhaFireball : public Projectile {
public:
    PiranhaFireball(StudentWorld &world, double startX, double startY,
                    int startDirection);

protected:
    bool shouldDamage(Actor *actor) override { return actor->isPeach(); }
};

class Shell : public Projectile {
public:
    Shell(StudentWorld &world, double startX, double startY,
          int startDirection);

protected:
    bool shouldDamage(Actor *actor) override {
        return actor->damageable() && !actor->isPeach();
    }
};

//==============================================================================
// Utility functions

bool areColliding(double xStart1, double yStart1, double xStart2, double yStart2);

#endif // ACTOR_H_
