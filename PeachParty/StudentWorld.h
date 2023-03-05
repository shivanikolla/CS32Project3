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
    ~StudentWorld();
    
//new functions
  bool boardisempty(int x, int y); //checks if a given square is empty
  bool isBlueCoinSquare(int x, int y);
    bool isRedCoinSquare(int x, int y);
    bool hasPlayer(int x, int y);
  void createVortex(int x, int y, int walkDirection);
    bool isUpDirSquare(int x, int y);
    bool isDownDirSquare(int x, int y);
    bool isRightDirSquare(int x, int y);
    bool isLeftDirSquare(int x, int y);
    bool isDirectionalSquare(int x, int y);
    void createDroppingSquare(int x, int y);
    
    PlayerAvatar* getPeach() {return p;}
    PlayerAvatar* getYoshi() {return yoshi;}
    
    bool PlayerLandsOnSquare(Square* a, PlayerAvatar* b);
    bool PlayerMovesOnSquare(Square* a, PlayerAvatar* b);
    bool PlayersOnSameSquare(Actor* a,Actor* b);
    int getBankAccountValue() { return bankAccountValue;}
    void setBankAccountValue(int value) {bankAccountValue += value; }
    void randomCoordinateGenerator();
    int getRandomX() const {return randomX;}
    int getRandomY() const {return randomY;}
    int getvortexWalkDirection() {return vortexWalkDirection;}
    void objectOverlapwithVortex(Vortex* v);
    void teleportBaddy(Baddy* b);
    
private:
    std::vector<Actor*> m_actor;
    PlayerAvatar* p; //to keep track of peach
    PlayerAvatar* yoshi; //to keep track of yoshi
    Board* b;
    Vortex* newVortex; //used for dynamically allocating a new vortex
    int bankAccountValue;
    int randomX;
    int randomY;
    int vortexWalkDirection;
};

#endif // STUDENTWORLD_H_

