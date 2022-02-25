#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//==============================================================================
// Class Actor

Actor::Actor(StudentWorld &world, int imageId, double startX, double startY,
             int startDirection, double size, int depth)
    : GraphObject(imageId, startX, startY, startDirection, depth, size),
      world(world) {}

void Actor::print() {
    std::cerr << "==== Actor ====" << std::endl;
    std::cerr << "Coordinates: (" << getX() << ", " << getY() << ")" << std::endl;
    std::cerr << "Passable: " << passable() << std::endl;
    std::cerr << "Movable: " << (movable() != nullptr) << std::endl;
}

//==============================================================================
// Class Movable

bool Movable::attemptMove(double dx, double dy) {
    this->dx += dx;
    this->dy += dy;
    if (!checkSpace()) {
        this->dx -= dx;
        this->dy -= dy;
        return false;
    }
    return true;
}

void Movable::addNearbyBlock(Actor *actor) {
    nearbyBlocks.push_back(actor);
}

bool Movable::checkSpace() {
    for (Actor *actor : nearbyBlocks) {
        BonkProps props;
        if (areColliding(actor->getX(), actor->getY(), getX() + dx, getY() + dy, &props)) {
            actor->bonk(this, props);
            return false;
        }
    }
    return true;
}

//==============================================================================
// Class Peach

Peach::Peach(StudentWorld &world, double startX, double startY)
    : Actor(world, IID_PEACH, startX, startY, 0, 1.0, 1) {}

void Peach::doSomething() {
    if (!hp)
        return;

    StudentWorld &world = getWorld();

    // Decrement Star Power
    // Decrement iframes
    // Decrement fireball delay
    // Hit any objects

    dx = dy = 0;

    if (jumpDist > 0) {
        // Jump logic
        if (attemptMove(0, 4)) {
            jumpDist--;
            grounded = false;
        } else {
            jumpDist = 0;
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
            attemptMove(-4, 0);
            break;
        case KEY_PRESS_RIGHT:
            setDirection(0);
            attemptMove(4, 0);
            break;
        case KEY_PRESS_UP:
            if (grounded)
                jumpDist = 8;
            break;
        case KEY_PRESS_SPACE:
            break;
        }
    }

    if (dx != 0 || dy != 0)
        moveTo(getX() + dx, getY() + dy);

    nearbyBlocks.clear();
}

void Peach::bonk(Actor *other, BonkProps props) {
}

//==============================================================================
// Class Block

Block::Block(StudentWorld &world, double startX, double startY) : Actor(world, IID_BLOCK, startX, startY, 0, 1.0, 2) {}

void Block::bonk(Actor *other, BonkProps props) {
    if (!props.top) {
        getWorld().playSound(SOUND_PLAYER_BONK);
        std::cerr << "SOUND PLAYER BONK" << std::endl;
    }
}

Pipe::Pipe(StudentWorld &world, double startX, double startY) : Block(world, startX, startY) {}

//==============================================================================
// Utility functions

inline bool areColliding(double x1, double y1, double x2, double y2, BonkProps *props1, BonkProps *props2) {
    double xStart1 = x1,
           yStart1 = y1,
           xEnd1 = xStart1 + SPRITE_WIDTH,
           yEnd1 = yStart1 + SPRITE_HEIGHT,
           xStart2 = x2,
           yStart2 = y2,
           xEnd2 = xStart2 + SPRITE_WIDTH,
           yEnd2 = yStart2 + SPRITE_HEIGHT;
    bool colliding = xEnd2 > xStart1 && xStart2 < xEnd1 && yEnd2 > yStart1 && yStart2 < yEnd1;
    if (colliding) {
        if (props1)
            *props1 = {(xStart2 < xStart1), (yStart2 < yStart1), (yEnd2 > yEnd1), (xEnd2 > xEnd1)};
        if (props2)
            *props2 = {(xStart1 < xStart2), (yStart1 < yStart2), (yEnd1 > yEnd2), (xEnd1 > xEnd2)};
    }
    return colliding;
}
