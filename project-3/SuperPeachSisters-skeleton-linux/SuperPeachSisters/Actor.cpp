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

void Actor::applySpaceProps(Surroundings newProps) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Surroundings::Pair &s = spaceProps.data[i][j],
                             &n = newProps.data[i][j];
            if (!s.actor || n.actor && n.space < s.space)
                s = n;
        }
    }
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
    if (dx < 0) {
        if (dy < 0) {
            return adx <= spaceProps.midLeft.space && ady <= spaceProps.botMid.space && adx + ady <= spaceProps.botLeft.space;
        } else if (dy > 0) {
            return adx <= spaceProps.midLeft.space && ady < spaceProps.topMid.space && adx + ady <= spaceProps.topLeft.space;
        } else {
            return adx <= spaceProps.midLeft.space;
        }
    } else if (dx > 0) {
        if (dy < 0) {
            return adx <= spaceProps.midRight.space && ady <= spaceProps.botMid.space && adx + ady <= spaceProps.botRight.space;
        } else if (dy > 0) {
            return adx <= spaceProps.midRight.space && ady <= spaceProps.topMid.space && adx + ady <= spaceProps.topRight.space;
        } else {
            return adx <= spaceProps.midRight.space;
        }
    } else {
        if (dy < 0) {
            return ady <= spaceProps.botMid.space;
        } else if (dy > 0) {
            return ady <= spaceProps.topMid.space;
        } else {
            return true;
        }
    }
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
    
    spaceProps.clear();
}

void Peach::bonk(Actor *other, BonkProps props) {
}

//==============================================================================
// Class Block

Block::Block(StudentWorld &world, double startX, double startY) : Actor(world, IID_BLOCK, startX, startY, 0, 1.0, 2) {}

void Block::doSomething() {
}

Pipe::Pipe(StudentWorld &world, double startX, double startY) : Actor(world, IID_PIPE, startX, startY, 0, 1.0, 2) {}
void Pipe::doSomething() {}

//==============================================================================
// Utility functions

void incrementSpace(Surroundings::Pair &data, Actor *actor, double space) {
    if (!data.actor) {
        data.actor = actor;
        data.space = space;
    } else {
        data.space += space;
    }
}

Surroundings calcSpace(Actor *actor1, Actor *actor2) {
    Surroundings ret;
    double xStart1 = actor1->getX(),
           yStart1 = actor1->getY(),
           xEnd1 = xStart1 + SPRITE_WIDTH,
           yEnd1 = yStart1 + SPRITE_HEIGHT,
           xStart2 = actor2->getX(),
           yStart2 = actor2->getY(),
           xEnd2 = xStart2 + SPRITE_WIDTH,
           yEnd2 = yStart2 + SPRITE_HEIGHT,
           space;
    if (xEnd2 <= xStart1) {
        space = xStart1 - xEnd2;
        if (yEnd2 > yEnd1) {
            incrementSpace(ret.topLeft, actor2, space);
        }
        if (yStart2 < yStart1) {
            incrementSpace(ret.botLeft, actor2, space);
        }
        if (yEnd2 > yStart1 && yStart2 < yEnd1) {
            incrementSpace(ret.midLeft, actor2, space);
        }
    }
    if (xStart2 >= xEnd1) {
        space = xStart2 - xEnd1;
        if (yEnd2 > yEnd1) {
            incrementSpace(ret.topRight, actor2, space);
        }
        if (yStart2 < yStart1) {
            incrementSpace(ret.botRight, actor2, space);
        }
        if (yEnd2 > yStart1 && yStart2 < yEnd1) {
            incrementSpace(ret.midRight, actor2, space);
        }
    }
    if (yStart2 >= yEnd1) {
        space = yStart2 - yEnd1;
        if (xEnd2 > xEnd1) {
            incrementSpace(ret.topRight, actor2, space);
        }
        if (xStart2 < xStart1) {
            incrementSpace(ret.topLeft, actor2, space);
        }
        if (xEnd2 > xStart1 && xStart2 < xEnd1) {
            incrementSpace(ret.topMid, actor2, space);
        }
    }
    if (yEnd2 <= yStart1) {
        space = yStart1 - yEnd2;
        if (xEnd2 > xEnd1) {
            incrementSpace(ret.botRight, actor2, space);
        }
        if (xStart2 < xStart1) {
            incrementSpace(ret.botLeft, actor2, space);
        }
        if (xEnd2 > xStart1 && xStart2 < xEnd1) {
            incrementSpace(ret.botMid, actor2, space);
        }
    }
    return ret;
}
