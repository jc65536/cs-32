#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
#include <list>
#include <sstream>
#include <string>

using namespace std;

GameWorld *createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath) {
}

int StudentWorld::init() {
    // Initialize provided level loader class
    Level level(assetPath());
    int levelNum = getLevel();
    string levelFilename = "level00.txt";
    levelFilename[5] = levelNum / 10 + '0';
    levelFilename[6] = levelNum % 10 + '0';
    Level::LoadResult loadRes = level.loadLevel(levelFilename);
    if (loadRes == Level::load_fail_bad_format) {
        cerr << "load fail bad format" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    if (loadRes == Level::load_fail_file_not_found) {
        cerr << "load fail file not found" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }

    // Loop through each grid position and decide what to do
    Level::GridEntry gridEntry;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            double x = j * SPRITE_WIDTH,
                   y = i * SPRITE_HEIGHT;
            gridEntry = level.getContentsOf(j, i);
            switch (gridEntry) {
            case Level::block:
                addActor<Block>(x, y);
                break;
            case Level::empty:
                break;
            case Level::flag:
                addActor<Flag>(x, y);
                break;
            case Level::flower_goodie_block:
                addActor<Block>(x, y)->setPowerup(Flower::create);
                break;
            case Level::goomba:
                break;
            case Level::koopa:
                break;
            case Level::mario:
                addActor<Mario>(x, y);
                break;
            case Level::mushroom_goodie_block:
                addActor<Block>(x, y)->setPowerup(Mushroom::create);
                break;
            case Level::peach:
                peach = addActor<Peach>(x, y);
                break;
            case Level::pipe:
                addActor<Pipe>(x, y);
                break;
            case Level::piranha:
                break;
            case Level::star_goodie_block:
                addActor<Block>(x, y)->setPowerup(Star::create);
                break;
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    returnCode = GWSTATUS_CONTINUE_GAME;

    auto it = actors.begin();
    while (it != actors.end()) {
        Actor *actor = *it;
        actor->doSomething();
        if (actor->isAlive()) {
            it++;
        } else {
            delete actor;
            it = actors.erase(it);
        }
    }

    ostringstream format;
    int powers = peach->getPowers();
    cerr << powers << endl;
    format << "Lives: " << getLives() <<
        "  Level: " << getLevel() <<
        "  Points: " << getScore() <<
        (powers & Peach::STAR ? "  StarPower!" : "") <<
        (powers & Peach::FIRE ? "  ShootPower!" : "") <<
        (powers & Peach::JUMP ? "  JumpPower!" : "") <<
        endl;
    setGameStatText(format.str());

    return returnCode;
}

list<Actor *> StudentWorld::findCollidingActors(Actor *self, double x, double y) {
    if (x < 0)
        x = self->getX();
    if (y < 0)
        y = self->getY();
    list<Actor *> collidingActors;
    for (Actor *actor : actors) {
        if (actor != self && areColliding(x, y, actor->getX(), actor->getY()))
            collidingActors.push_back(actor);
    }
    return collidingActors;
}

void StudentWorld::cleanUp() {
    for (Actor *actor : actors)
        delete actor;
    actors.clear();
}
