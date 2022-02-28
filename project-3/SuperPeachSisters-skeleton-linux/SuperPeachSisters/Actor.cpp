#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <cmath>
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
    double newX = getX() + dx, newY = getY() + dy;
    if (testPosition(newX, newY, bonk)) {
        moveTo(newX, newY);
        return true;
    } else {
        return false;
    }
}

bool Actor::testPosition(double x, double y, bool bonk) {
    list<Actor *> collidingActors = world.findCollidingActors(this, x, y);
    for (Actor *actor : collidingActors) {
        if (!actor->passable()) {
            if (bonk)
                actor->bonk(this);
            return false;
        }
    }
    return true;
}

bool Actor::isPeach() {
    return this == world.getPeach();
}

bool Actor::overlappingWithPeach() {
    Peach *peach = world.getPeach();
    return areColliding(getX(), getY(), peach->getX(), peach->getY());
}

//==============================================================================
// Peach

Peach::Peach(StudentWorld &world, double startX, double startY)
    : Actor(world, IID_PEACH, startX, startY, 0, 0, 1.0),
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

    if (starCountdown > 0) {
        starCountdown--;
        if (starCountdown == 0)
            powers &= ~Peach::STAR;
    }

    // Invincibility status is implied by checking if the countdown > 0
    if (invincibilityCountdown > 0)
        invincibilityCountdown--;

    if (fireCountdown > 0)
        fireCountdown--;

    // Bonk any objects
    list<Actor *> collidingActors = world.findCollidingActors(this);
    for (Actor *actor : collidingActors) {
        actor->bonk(this);
    }

    if (jumpDistance > 0) {
        if (attemptMove(0, 4, true)) {
            jumpDistance--;
            grounded = false;
        } else {
            jumpDistance = 0;
        }
    } else {
        // Fall if not jumping
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
            if (grounded) {
                jumpDistance = hasPower(Peach::JUMP) ? 12 : 8;
                world.playSound(SOUND_PLAYER_JUMP);
                cerr << "SOUND_PLAYER_JUMP" << endl;
            }
            break;
        case KEY_PRESS_SPACE:
            if (hasPower(Peach::FIRE) && fireCountdown == 0) {
                world.playSound(SOUND_PLAYER_FIRE);
                cerr << "SOUND_PLAYER_FIRE" << endl;
                fireCountdown = 8;
                int dir = getDirection();
                world.addActor(new PeachFireball(world, getX() + (dir == 0 ? 4 : -4), getY(), dir));
            }
            break;
        }
    }
}

void Peach::addPower(int power) {
    powers |= power;
    if (power == Peach::STAR)
        starCountdown = 150;
    else
        hp = 2;
}

void Peach::bonk(Actor *other) {
    takeDamage();
}

void Peach::takeDamage() {
    if (hasPower(Peach::STAR) || invincibilityCountdown > 0)
        return;

    hp--;
    invincibilityCountdown = 10;
    powers = 0;

    if (hp > 0) {
        getWorld().playSound(SOUND_PLAYER_HURT);
        cerr << "SOUND_PLAYER_HURT" << endl;
    } else {
        die();
    }
}

//==============================================================================
// Obstacles

Pipe::Pipe(StudentWorld &world, double startX, double startY, int imageId)
    : Actor(world, imageId, startX, startY, 0, 2, 1.0) {}

Block::Block(StudentWorld &world, double startX, double startY, bool hasPowerup)
    : Pipe(world, startX, startY, IID_BLOCK),
      hasPowerup(hasPowerup) {}

void Block::bonk(Actor *other) {
    StudentWorld &world = getWorld();
    if (hasPowerup) {
        world.playSound(SOUND_POWERUP_APPEARS);
        cerr << "SOUND_POWERUP_APPEARS" << endl;
        world.addActor(makePowerup());
        hasPowerup = false;
    } else {
        world.playSound(SOUND_PLAYER_BONK);
        cerr << "SOUND_PLAYER_BONK" << endl;
    }
}

//==============================================================================
// Flags

Flag::Flag(StudentWorld &world, double startX, double startY, int imageId)
    : Actor(world, imageId, startX, startY, 0, 1, 1.0) {}

void Flag::doSomething() {
    if (isAlive() && overlappingWithPeach()) {
        StudentWorld &world = getWorld();
        world.increaseScore(1000);
        die();
        world.setStatus(status());
    }
}

Mario::Mario(StudentWorld &world, double startX, double startY)
    : Flag(world, startX, startY, IID_MARIO) {}

//==============================================================================
// Powerups

Powerup::Powerup(StudentWorld &world, int imageId, double startX, double startY)
    : Actor(world, imageId, startX, startY, 0, 1, 1.0) {}

void Powerup::doSomething() {
    StudentWorld &world = getWorld();
    if (overlappingWithPeach()) {
        world.increaseScore(points());
        world.getPeach()->addPower(power());
        die();
        world.playSound(SOUND_PLAYER_POWERUP);
        cerr << "SOUND_PLAYER_POWERUP" << endl;
        return;
    }

    attemptMove(0, -2);

    bool right = getDirection() == 0;
    int dx = right ? 2 : -2;
    if (!attemptMove(dx, 0))
        setDirection(right ? 180 : 0);
}

Flower::Flower(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_FLOWER, startX, startY) {}

Mushroom::Mushroom(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_MUSHROOM, startX, startY) {}

Star::Star(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_STAR, startX, startY) {}

//==============================================================================
// Enemies

Enemy::Enemy(StudentWorld &world, int imageId, double startX, double startY)
    : Actor(world, imageId, startX, startY, randInt(0, 1) * 180, 1.0, 0),
      minX(0),
      maxX(VIEW_WIDTH - SPRITE_WIDTH) {}

void Enemy::bonk(Actor *other) {
    if (!other->isPeach())
        return;

    StudentWorld &world = getWorld();
    Peach *peach = world.getPeach();

    if (peach->hasPower(Peach::STAR)) {
        world.playSound(SOUND_PLAYER_KICK);
        cerr << "SOUND_PLAYER_KICK" << endl;
        takeDamage();
    }
}

void Enemy::takeDamage() {
    getWorld().increaseScore(100);
    die();
}

Goomba::Goomba(StudentWorld &world, double startX, double startY, int imageId)
    : Enemy(world, imageId, startX, startY) {}

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
    if (!attemptMove(dx, 0))
        setDirection(right ? 180 : 0);
}

Koopa::Koopa(StudentWorld &world, double startX, double startY)
    : Goomba(world, startX, startY, IID_KOOPA) {}

void Koopa::takeDamage() {
    Enemy::takeDamage();
    StudentWorld &world = getWorld();
    world.addActor(new Shell(world, getX(), getY(), getDirection()));
}

Piranha::Piranha(StudentWorld &world, double startX, double startY)
    : Enemy(world, IID_PIRANHA, startX, startY),
      fireCountdown(0) {}

void Piranha::doSomething() {
    if (!isAlive())
        return;

    increaseAnimationNumber();

    StudentWorld &world = getWorld();
    Peach *peach = world.getPeach();

    if (overlappingWithPeach()) {
        peach->bonk(this);
        return;
    }

    if (abs(peach->getY() - getY()) >= 1.5 * SPRITE_HEIGHT)
        return;

    int xDifference = peach->getX() - getX();
    setDirection(xDifference > 0 ? 0 : 180);

    if (fireCountdown) {
        fireCountdown--;
        return;
    }

    if (abs(xDifference) < 8 * SPRITE_WIDTH) {
        world.addActor(new PiranhaFireball(world, getX(), getY(), getDirection()));
        world.playSound(SOUND_PIRANHA_FIRE);
        cerr << "SOUND_PIRANHA_FIRE" << endl;
        fireCountdown = 40;
    }
}

//==============================================================================
// Projectiles

Projectile::Projectile(StudentWorld &world, int imageId,
                       double startX, double startY, int startDirection)
    : Actor(world, imageId, startX, startY, startDirection, 1, 1.0) {}

void Projectile::doSomething() {
    StudentWorld &world = getWorld();
    list<Actor *> collidingActors = world.findCollidingActors(this);
    for (Actor *actor : collidingActors) {
        if (shouldDamage(actor)) {
            actor->takeDamage();
            die();
            return;
        }
    }

    attemptMove(0, -2);

    double dx = getDirection() == 0 ? 2 : -2;
    if (!attemptMove(dx, 0))
        die();
}

PeachFireball::PeachFireball(StudentWorld &world, double startX, double startY,
                             int startDirection)
    : Projectile(world, IID_PEACH_FIRE, startX, startY, startDirection) {}

PiranhaFireball::PiranhaFireball(StudentWorld &world, double startX, double startY,
                                 int startDirection)
    : Projectile(world, IID_PIRANHA_FIRE, startX, startY, startDirection) {}

Shell::Shell(StudentWorld &world, double startX, double startY,
             int startDirection)
    : Projectile(world, IID_SHELL, startX, startY, startDirection) {}
