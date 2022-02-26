#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <iostream>
#include <list>

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//==============================================================================
// Actor

Actor::Actor(StudentWorld &world, int imageId, double startX, double startY,
             int startDirection, int depth, double size)
    : GraphObject(imageId, startX, startY, startDirection, depth, size),
      world(world),
      alive(true) {}

void Actor::print() {
    cerr << "==== Actor ====" << endl;
    cerr << "Coordinates: (" << getX() << ", " << getY() << ")" << endl;
    cerr << "Passable: " << passable() << endl;
}

bool Actor::attemptMove(double dx, double dy, bool bonk) {
    double newX = getX() + dx, newY = getY() + dy;
    list<Actor *> collidingActors = getWorld().findCollidingActors(this, newX, newY);
    for (Actor *actor : collidingActors) {
        if (!actor->passable()) {
            if (bonk)
                actor->bonk(this);
            return false;
        }
    }
    moveTo(newX, newY);
    return true;
}

inline bool Actor::overlapsWithPeach() {
    Peach *peach = world.getPeach();
    return areColliding(getX(), getY(), peach->getX(), peach->getY());
}

//==============================================================================
// Peach

Peach::Peach(StudentWorld &world, double startX, double startY)
    : Actor(world, IID_PEACH, startX, startY, 0, 1, 1.0) {}

void Peach::doSomething() {
    if (!isAlive())
        return;

    StudentWorld &world = getWorld();

    // Decrement Star Power
    // Decrement iframes
    // Decrement fireball delay
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
                jumpDistance = 8;
            break;
        case KEY_PRESS_SPACE:
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

Block::Block(StudentWorld &world, double startX, double startY)
    : Pipe(world, startX, startY, IID_BLOCK),
      createPowerup(nullptr) {}

void Block::setPowerup(void (*create)(StudentWorld &, double, double)) {
    createPowerup = create;
}

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
    if (overlapsWithPeach()) {
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
    if (overlapsWithPeach()) {
        // Increase score by points()
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
    world.addActor<Flower>(startX, startY);
}

Mushroom::Mushroom(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_MUSHROOM, startX, startY) {}

void Mushroom::create(StudentWorld &world, double startX, double startY) {
    world.addActor<Mushroom>(startX, startY);
}

Star::Star(StudentWorld &world, double startX, double startY)
    : Powerup(world, IID_STAR, startX, startY) {}

void Star::create(StudentWorld &world, double startX, double startY) {
    world.addActor<Star>(startX, startY);
}

//==============================================================================
// Utility functions

bool areColliding(double xStart1, double yStart1, double xStart2, double yStart2) {
    double xEnd1 = xStart1 + SPRITE_WIDTH,
           yEnd1 = yStart1 + SPRITE_HEIGHT,
           xEnd2 = xStart2 + SPRITE_WIDTH,
           yEnd2 = yStart2 + SPRITE_HEIGHT;
    return xEnd2 > xStart1 && xStart2 < xEnd1 && yEnd2 > yStart1 && yStart2 < yEnd1;
}
