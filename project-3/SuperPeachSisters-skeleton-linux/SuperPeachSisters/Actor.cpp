#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, double startX, double startY,
             int startDirection = 0, double size = 1.0, int depth = 0)
    : GraphObject(imageId, startX, startY, startDirection, depth, size) {}

Peach::Peach() : Actor(IID_PEACH, 0, 0) {}

Block::Block() : Actor(IID_BLOCK, 4, 4) {}
