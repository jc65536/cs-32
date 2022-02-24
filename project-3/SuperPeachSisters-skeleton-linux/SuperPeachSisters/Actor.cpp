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

void Actor::addSurroundings(Surroundings newProps) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Surroundings::Pair &s = spaceProps.data[i][j],
                               &n = newProps.data[i][j];
            if (!s.actor || n.actor && n.dist < s.dist)
                s = n;
        }
    }
}

void Actor::print() {
    std::cerr << "==== Actor ====" << std::endl;
    std::cerr << "Coordinates: (" << getX() << ", " << getY() << ")" << std::endl;
    std::cerr << "Passable: " << passable() << std::endl;
    std::cerr << "Movable: " << movable() << std::endl;
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

bool Movable::checkSpace() {
    double adx = std::abs(dx),
           ady = std::abs(dy);
    
    bool dxDirection[] = {dx < 0, true, dx > 0};
    bool dyDirection[] = {dy > 0, true, dy < 0};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) // Do not check middle square
                continue;
            // Whether to check y movement
            bool yDecision = dyDirection[i];
            // Whether to check x movement
            bool xDecision = dxDirection[j];
            if (yDecision && xDecision) {
                double attemptDist = yDecision * ady + xDecision * adx;
                if (attemptDist > spaceProps.data[i][j].dist)
                    return false;
            }
        }
    }

    /*
    dy > 0 && dx < 0 | dy > 0 && true | dy > 0 && dx > 0
    true   && dx < 0 | false          | true   && dx > 0
    dy < 0 && dx < 0 | dy < 0 && true | dy < 0 && dx > 0
    */

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
        if (attemptMove(0, 4))
            jumpDist--;
        else
            jumpDist = 0;
    } else {
        // Fall logic
        attemptMove(0, -4);
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
            jumpDist = 8;
            break;
        case KEY_PRESS_SPACE:
            break;
        }
    }

    if (dx != 0 || dy != 0)
        moveTo(getX() + dx, getY() + dy);

    std::cerr << "====" << std::endl;
    spaceProps.print();
    spaceProps.clear();
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

Surroundings calcSpace(Actor *actor1, Actor *actor2) {
    Surroundings ret;
    double xStart1 = actor1->getX(),
           yStart1 = actor1->getY(),
           xEnd1 = xStart1 + SPRITE_WIDTH,
           yEnd1 = yStart1 + SPRITE_HEIGHT,
           xStart2 = actor2->getX(),
           yStart2 = actor2->getY(),
           xEnd2 = xStart2 + SPRITE_WIDTH,
           yEnd2 = yStart2 + SPRITE_HEIGHT;

    // x distance between the actors if actor2 is on actor1's left/middle/right
    double xDist[] = {xStart1 - xEnd2, -1, xStart2 - xEnd1};
    // y distance between the actors if actor2 is on actor1's top/middle/bottom
    double yDist[] = {yStart2 - yEnd1, -1, yStart1 - yEnd2};

    // Whether any part of actor2 exists in the top/middle/bottom row of the
    // space around actor1
    bool inCol[3] = {xStart2 < xStart1, xEnd2 > xStart1 && xStart2 < xEnd1, xEnd2 > xEnd1};
    // Whether any part of actor2 exists in the left/middle/right column of the
    // space around actor1
    bool inRow[3] = {yEnd2 > yEnd1, yEnd2 > yStart1 && yStart2 < yEnd1, yStart2 < yStart1};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Should y distance be added
            bool yDecision = yDist[i] >= 0 && inCol[j];
            // Should x distance be added
            bool xDecision = xDist[j] >= 0 && inRow[i];
            if (xDecision || yDecision) {
                Surroundings::Pair &data = ret.data[i][j];
                data.actor = actor2;
                data.dist = yDecision * yDist[i] + xDecision * xDist[j];
            }
        }
    }

    /*
    (xe2 <= xs1 && ye2 > ye1) * left + (ys2 >= ye1 && xs2 < xs1) * top | (ys2 >= ye1 && xe2 > xs1 && xs2 < xe1) * top | (xs2 >= xe1 && ye2 > ye1) * right + (ys2 >= ye1 && xe2 > xe1) * right
    (xe2 <= xs1 && ye2 > ys1 && ys2 < ye1) * left                      | -1                                           | (xs2 >= xe1 && ye2 > ys1 && ys2 < ye1) * right
    (xe2 <= xs1 && ys2 < ys1) * left + (ye2 <= ys1 && xs2 < xs1) * bot | (ye2 <= ys1 && xe2 > xs1 && xs2 < xe1) * bot | (xs2 >= xe1 && ys2 < ys1) * right + (ye2 <= ys1 && xe2 > xe1) * bot
    */

    return ret;
}
