#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <iostream>
#include <list>
#include <random>

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//==============================================================================
// Utility functions

bool areColliding(double xStart1, double yStart1, double xStart2, double yStart2) {
    double xEnd1 = xStart1 + SPRITE_WIDTH,
           yEnd1 = yStart1 + SPRITE_HEIGHT,
           xEnd2 = xStart2 + SPRITE_WIDTH,
           yEnd2 = yStart2 + SPRITE_HEIGHT;
    return xEnd2 > xStart1 && xStart2 < xEnd1 && yEnd2 > yStart1 && yStart2 < yEnd1;
}

//==============================================================================
// Actor

Actor::Actor(StudentWorld &world, int imageId, double startX, double startY,
             int startDirection, int depth, double size)
    : GraphObject(imageId, startX, startY, startDirection, depth, size),
      world(world),
      alive(true) {}

bool Actor::attemptMove(double dx, double dy, bool bonk) {
    bool attemptSuccessful = true;
    double newX = getX() + dx, newY = getY() + dy;
    if (testPosition(newX, newY, bonk)) {
        moveTo(newX, newY);
        return true;
    } else {
        return false;
    }
}

bool Actor::testPosition(double x, double y, bool bonk) {
    bool result = true;
    list<Actor *> collidingActors = getWorld().findCollidingActors(this, x, y);
    for (Actor *actor : collidingActors) {
        if (!actor->passable()) {
            if (bonk)
                actor->bonk(this);
            result = false;
        }
    }
    return result;
}

bool Actor::isPeach() {
    return this == getWorld().getPeach();
}

bool Actor::overlappingWithPeach() {
    Peach *peach = world.getPeach();
    return areColliding(getX(), getY(), peach->getX(), peach->getY());
}

//==============================================================================
// Peach

Peach::Peach(StudentWorld &world, double startX, double startY)
    : Actor(world, IID_PEACH, startX, startY, 0, 1, 1.0),
      hp(1),
      powers(0),
      jumpDistance(0),
      fireCountdown(0),
      starCountdown(0),
      grounded(true) {}

void Peach::doSomething() {
    if (!isAlive())
        return;

    StudentWorld &world = getWorld();

    // Decrement Star Power
    // Decrement iframes

    fireCountdown &&fireCountdown--;

    // Hit any objects

    list<Actor *> collidingActors = world.findCollidingActors(this);
    for (Actor *actor : collidingActors) {
        actor->bonk(this);
    }

    if (jumpDistance > 0) {
        // Jump logic
        if (attemptMove(0, 4, true)) {
            jumpDistance--;
            grounded = false;
        } else {
            jumpDistance = 0;
        }
    } else {
        // Fall logic
        grounded = !attemptMove(0, -4);
    }

    int key;
    if (world.getKey(key)) {
        switch (key) {
        case KEY_PRESS_LEFT:
            setDirection(180);
            attemptMove(-4, 0, true);
            break;
        case KEY_PRESS_RIGHT:
            setDirection(0);
            attemptMove(4, 0, true);
            break;
        case KEY_PRESS_UP:
            if (grounded)
                jumpDistance = powers & Peach::JUMP ? 12 : 8;
            break;
        case KEY_PRESS_SPACE:
            if (powers & Peach::FIRE && !fireCountdown) {
                world.playSound(SOUND_PLAYER_FIRE);
                fireCountdown = 8;
                int dir = getDirection();
                world.addActor(new PeachFireball(world, getX() + (dir == 0 ? 4 : -4), getY(), dir));
            }
            break;
        }
    }
}

void Peach::bonk(Actor *other) {
}

//==============================================================================
// Obstacles

Pipe::Pipe(StudentWorld &world, double startX, double startY, int imageId)
    : Actor(world, imageId, startX, startY, 0, 2, 1.0) {}

Block::Block(StudentWorld &world, double startX, double startY, void (*create)(StudentWorld &, double, double))
    : Pipe(world, startX, startY, IID_BLOCK),
      createPowerup(create) {}

void Block::bonk(Actor *other) {
    StudentWorld &world = getWorld();
    if (createPowerup) {
        world.playSound(SOUND_POWERUP_APPEARS);
        cerr << "SOUND_POWERUP_APPEARS" << endl;
        createPowerup(world, getX(), getY() + 8);
        createPowerup = nullptr;
    } else {
        world.playSound(SOUND_PLAYER_BONK);
        cerr << "SOUND_PLAYER_BONK" << endl;
    }
}

//==============================================================================
// Flag

Flag::Flag(StudentWorld &world, double startX, double startY, int imageId)
    : Actor(world, imageId, startX, startY, 0, 1, 1.0) {}

void Flag::doSomething() {
    if (overlappingWithPeach()) {
        getWorld().increaseScore(1000);
        gameSignal();
        die();
    }
}

void Flag::gameSignal() {
    getWorld().finishLevel();
}

Mario::Mario(StudentWorld &world, double startX, double startY)
    : Flag(world, startX, startY, IID_MARIO) {}

void Mario::gameSignal() {
    getWorld().winGame();
}

//==============================================================================
// Powerups

Powerup::Powerup(StudentWorld &world, int imageId, double startX, double startY)
    : Actor(world, imageId, startX, startY, 0, 1, 1.0) {}

void Powerup::doSomething() {
    StudentWorld &world = getWorld();
    if (overlappingWithPeach()) {
        // Increase score by points()
        world.increaseScore(points());
        Peach *peach = world.getPeach();
        peach->setHp(2);
        peach->addPower(power());
        die();
        world.playSound(SOUND_PLAYER_POWERUP);
        cerr << "SOUND_PLAYER_POWERUP" << endl;
        return;
    }

    attemptMove(0, -2);

    bool right = getDirection() == 0;
    int dx = right ? 2 : -2;
    if (!attemptMove(dx, 0)) {
        setDirection(right ? 180 : 0);
    }
}

Flower::Flower(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_FLOWER, startX, startY) {}

void Flower::create(StudentWorld &world, double startX, double startY) {
    world.addActor(new Flower(world, startX, startY));
}

Mushroom::Mushroom(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_MUSHROOM, startX, startY) {}

void Mushroom::create(StudentWorld &world, double startX, double startY) {
    world.addActor(new Mushroom(world, startX, startY));
}

Star::Star(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_STAR, startX, startY) {}

void Star::create(StudentWorld &world, double startX, double startY) {
    world.addActor(new Star(world, startX, startY));
}

//==============================================================================
// Enemies

Enemy::Enemy(StudentWorld &world, double startX, double startY, int imageId)
    : Actor(world, imageId, startX, startY, randInt(0, 1) * 180, 1.0, 0),
      minX(0),
      maxX(VIEW_WIDTH - SPRITE_WIDTH) {}

void Enemy::bonk(Actor *other) {
    if (!other->isPeach())
        return;

    StudentWorld &world = getWorld();
    Peach *peach = world.getPeach();

    if (peach->getPowers() & Peach::STAR) {
        world.playSound(SOUND_PLAYER_KICK);
        takeDamage();
    }
}

void Enemy::takeDamage() {
    getWorld().increaseScore(100);
    die();
}

Goomba::Goomba(StudentWorld &world, double startX, double startY, int imageId)
    : Enemy(world, startX, startY, imageId) {}

bool Goomba::testPosition(double x, double y, bool bonk) {
    return getMinX() <= x && x <= getMaxX() && Actor::testPosition(x, y, bonk);
}

void Goomba::doSomething() {
    if (!isAlive())
        return;

    StudentWorld &world = getWorld();

    if (overlappingWithPeach()) {
        world.getPeach()->bonk(this);
        return;
    }

    bool right = getDirection() == 0;
    double dx = right ? 1 : -1;
    if (!attemptMove(dx, 0, false))
        setDirection(right ? 180 : 0);
}

Koopa::Koopa(StudentWorld &world, double startX, double startY)
    : Goomba(world, startX, startY, IID_KOOPA) {}

//==============================================================================
// Projectiles

Projectile::Projectile(StudentWorld &world, int imageId,
                       double startX, double startY, int startDirection)
    : Actor(world, imageId, startX, startY, startDirection, 1, 1.0) {}

void Projectile::doSomething() {
    StudentWorld &world = getWorld();
    list<Actor *> collidingActors = world.findCollidingActors(this);
    for (Actor *actor : collidingActors) {
        if (shouldDamage(actor))
            actor->bonk(this);
    }

    attemptMove(0, -2);

    double dx = getDirection() == 0 ? 2 : -2;
    if (!attemptMove(dx, 0))
        die();
}

PeachFireball::PeachFireball(StudentWorld &world, double startX, double startY, int startDirection)
    : Projectile(world, IID_PEACH_FIRE, startX, startY, startDirection) {}
