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
    for (Actor *actor : actors) {
        actor->doSomething();
    }
    for (int i = 0; i + 1 < actors.size(); i++) {
        Actor *actor1 = actors[i];
        for (int j = i + 1; j < actors.size(); j++) {
            Actor *actor2 = actors[j];
            double xStart1 = actor1->getX(),
                   yStart1 = actor1->getY(),
                   xEnd1 = xStart1 + SPRITE_WIDTH,
                   yEnd1 = yStart1 + SPRITE_HEIGHT,
                   xStart2 = actor2->getX(),
                   yStart2 = actor2->getY(),
                   xEnd2 = xStart2 + SPRITE_WIDTH,
                   yEnd2 = yStart2 + SPRITE_HEIGHT;
            if (xEnd2 > xStart1 && xStart2 < xEnd1 && yEnd2 > yStart1 && yStart2 < yEnd1) {
                BonkProps props;
                props.left = xStart2 < xStart1;
                props.bot = yStart2 < yStart1;
                props.top = yEnd2 > yEnd1;
                props.right = xEnd2 > xEnd1;
                actor1->bonk(actor2, props);
                props.left = xStart1 < xStart2;
                props.bot = yStart1 < yStart2;
                props.top = yEnd1 > yEnd2;
                props.right = xEnd1 > xEnd2;
                actor2->bonk(actor1, props);
            } else if (actor1->movable() && !actor2->passable()) {
                actor1->applySpaceProps(calcSpace(actor1, actor2));
            } else if (!actor1->passable() && actor2->movable()) {
                actor2->applySpaceProps(calcSpace(actor2, actor1));
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
}
