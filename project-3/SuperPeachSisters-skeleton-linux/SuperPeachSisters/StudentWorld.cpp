#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
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
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            double pxX = x * SPRITE_WIDTH;
            double pxY = y * SPRITE_HEIGHT;
            gridEntry = level.getContentsOf(x, y);
            switch (gridEntry) {
            case Level::block:
                actors.push_back(new Block(*this, pxX, pxY));
                break;
            case Level::empty:
                break;
            case Level::flag:
                break;
            case Level::flower_goodie_block:
                actors.push_back(new Block(*this, pxX, pxY));
                break;
            case Level::goomba:
                break;
            case Level::koopa:
                break;
            case Level::mario:
                break;
            case Level::mushroom_goodie_block:
                actors.push_back(new Block(*this, pxX, pxY));
                break;
            case Level::peach:
                peach = new Peach(*this, pxX, pxY);
                actors.push_back(peach);
                break;
            case Level::pipe:
                actors.push_back(new Pipe(*this, pxX, pxY));
                break;
            case Level::piranha:
                break;
            case Level::star_goodie_block:
                actors.push_back(new Block(*this, pxX, pxY));
                break;
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    for (int i = 0; i + 1 < actors.size(); i++) {
        Actor *actor1 = actors[i];
        for (int j = i + 1; j < actors.size(); j++) {
            Actor *actor2 = actors[j];
            BonkProps props1, props2;
            if (areColliding(actor1->getX(), actor1->getY(), actor2->getX(), actor2->getY(), &props1, &props2)) {
                actor1->bonk(actor2, props1);
                actor2->bonk(actor1, props2);
                continue;
            }

            if (!actor1->passable() && actor2->movable()) {
                actor1 = actors[j];
                actor2 = actors[i];
            }

            if (actor1->movable() && !actor2->passable() && areNearby(actor1, actor2)) {
                actor1->movable()->addNearbyActor(actor2);
            }
        }
    }

    for (Actor *actor : actors) {
        actor->doSomething();
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    for (Actor *actor : actors)
        delete actor;
}
