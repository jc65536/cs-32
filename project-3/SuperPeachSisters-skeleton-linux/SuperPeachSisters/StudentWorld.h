#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "Level.h"
#include <list>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    int init() override;
    int move() override;
    void cleanUp() override;
    ~StudentWorld();

    std::list<Actor *> findCollidingActors(Actor *self, double x = -1, double y = -1);
    Peach *getPeach() { return peach; }
    void addActor(Actor *actor) { actors.push_back(actor); }
    void setStatus(int status) { this->status = status; }

private:
    std::list<Actor *> actors;
    Peach *peach;
    int status;
};

#endif // STUDENTWORLD_H_
