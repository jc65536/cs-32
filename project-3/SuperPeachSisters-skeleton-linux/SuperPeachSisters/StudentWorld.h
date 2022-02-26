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
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    std::list<Actor *> findCollidingActors(Actor *self, double x = -1, double y = -1);
    Peach *getPeach() { return peach; }

    void finishLevel() { returnCode = GWSTATUS_FINISHED_LEVEL; }
    void winGame() { returnCode = GWSTATUS_PLAYER_WON; }

    template <typename T>
    T *addActor(double startX, double startY) {
        T *actor = new T(*this, startX, startY);
        actors.push_back(actor);
        return actor;
    }

private:
    std::list<Actor *> actors;
    Peach *peach;
    int returnCode;
};

#endif // STUDENTWORLD_H_
