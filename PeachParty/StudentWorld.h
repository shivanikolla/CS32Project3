#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <vector>
#include <string>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
Actor* New(int imageID, int startX, int startY, int dir, int depth, std::string className);
    
private:
    std::vector<Actor*> m_actor;
    Peach* p;
    Yoshi* yoshi;
};

#endif // STUDENTWORLD_H_

