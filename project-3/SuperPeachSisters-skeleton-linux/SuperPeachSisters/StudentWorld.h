#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  // The 4 least significant bits of the returned int represent whether the
  // left, right, bottom, or top edge of target is within with
  int findIntersections(Actor *target, bool applyBonk = false);

private:
  std::vector<Actor *> actors;
  Peach *peach;
};

#endif // STUDENTWORLD_H_
