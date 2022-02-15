#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(int imageId, double startX, double startY,
          int startDirection = 0, double size = 1.0, int depth = 0);
};

class Peach : public Actor {
public:
    Peach();
};

class Block : public Actor {
public:
    Block();
};

#endif // ACTOR_H_
