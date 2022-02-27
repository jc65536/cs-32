#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
#include <list>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

GameWorld *createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath) {
}

template <typename T>
void spawnPowerup(StudentWorld &world, double startX, double startY) {
    world.addActor(new T(world, startX, startY));
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
    list<pair<double, double>> previousStretches, currentStretches;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        currentStretches = {{0, -1}};

        for (int j = 0; j < GRID_WIDTH; j++) {
            double x = j * SPRITE_WIDTH,
                   y = i * SPRITE_HEIGHT;
            gridEntry = level.getContentsOf(j, i);
            Enemy *enemy = nullptr;
            bool empty = false;

            switch (gridEntry) {
            case Level::block:
                addActor(new Block(*this, x, y));
                break;
            case Level::empty:
                empty = true;
                break;
            case Level::flag:
                addActor(new Flag(*this, x, y));
                break;
            case Level::flower_goodie_block:
                addActor(new Block(*this, x, y, spawnPowerup<Flower>));
                break;
            case Level::goomba:
                enemy = new Goomba(*this, x, y);
                addActor(enemy);
                break;
            case Level::koopa:
                enemy = new Koopa(*this, x, y);
                addActor(enemy);
                break;
            case Level::mario:
                addActor(new Mario(*this, x, y));
                break;
            case Level::mushroom_goodie_block:
                addActor(new Block(*this, x, y, spawnPowerup<Mushroom>));
                break;
            case Level::peach:
                peach = new Peach(*this, x, y);
                addActor(peach);
                break;
            case Level::pipe:
                addActor(new Pipe(*this, x, y));
                break;
            case Level::piranha:
                addActor(new Piranha(*this, x, y));
                break;
            case Level::star_goodie_block:
                addActor(new Block(*this, x, y, spawnPowerup<Star>));
                break;
            }

            if (enemy) {
                pair<double, double> stretch;
                while (!previousStretches.empty()) {
                    stretch = previousStretches.front();
                    if (stretch.first <= x && stretch.second > x) {
                        enemy->setMinX(stretch.first);
                        enemy->setMaxX(stretch.second - SPRITE_WIDTH);
                        break;
                    }
                    previousStretches.pop_front();
                }
            }

            pair<double, double> &stretch = currentStretches.back();
            bool passable = empty || actors.back()->passable();
            if (stretch.second < 0 && passable) {
                // Terminate stretch on a passable square
                stretch.second = x;
            } else if (stretch.second >= 0 && !passable) {
                // Start a new stretch on a blocking square
                currentStretches.push_back({x, -1});
            }
        }

        currentStretches.back().second = VIEW_WIDTH;
        previousStretches = currentStretches;
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    status = GWSTATUS_CONTINUE_GAME;

    auto it = actors.begin();
    while (it != actors.end()) {
        Actor *actor = *it;
        actor->doSomething();
        if (actor->isAlive()) {
            it++;
        } else if (actor == peach) {
            playSound(SOUND_PLAYER_DIE);
            cerr << "SOUND_PLAYER_DIE" << endl;
            decLives();
            return GWSTATUS_PLAYER_DIED;
        } else {
            delete actor;
            it = actors.erase(it);
        }
    }

    ostringstream format;
    format << "Lives: " << getLives() <<
        "  Level: " << getLevel() <<
        "  Points: " << getScore() <<
        (peach->hasPower(Peach::STAR) ? "  StarPower!" : "") <<
        (peach->hasPower(Peach::FIRE) ? "  ShootPower!" : "") <<
        (peach->hasPower(Peach::JUMP) ? "  JumpPower!" : "") <<
        endl;
    setGameStatText(format.str());

    return status;
}

list<Actor *> StudentWorld::findCollidingActors(Actor *self, double x, double y) {
    if (x < 0)
        x = self->getX();
    if (y < 0)
        y = self->getY();
    list<Actor *> collidingActors;
    for (Actor *actor : actors) {
        if (actor != self && actor->isAlive() && areColliding(x, y, actor->getX(), actor->getY()))
            collidingActors.push_back(actor);
    }
    return collidingActors;
}

void StudentWorld::cleanUp() {
    for (Actor *actor : actors)
        delete actor;
    actors.clear();
}

StudentWorld::~StudentWorld() {
    cleanUp();
}
