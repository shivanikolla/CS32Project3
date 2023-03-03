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
//  Board* getBoard() const {return b;}
  void createVortex(int x, int y);
    bool isUpDirSquare(int x, int y);
    bool isDownDirSquare(int x, int y);
    bool isRightDirSquare(int x, int y);
    bool isLeftDirSquare(int x, int y);
    PlayerAvatar* getPeach() {return p;}
    PlayerAvatar* getYoshi() {return yoshi;}
    bool PlayerLandsOnSquare(Square* a, PlayerAvatar* b);
    bool PlayerMovesOnSquare(Square* a, PlayerAvatar* b);
    
    //actually make two different methods: one that checks if the player has "moved" onto the square and one that checks if the player has "landed" on the square(waiting to roll state equals true
    
private:
    std::vector<Actor*> m_actor;
    PlayerAvatar* p; //to keep track of peach
    PlayerAvatar* yoshi; //to keep track of yoshi
    Board* b;
    Vortex* newVortex; //used for dynamically allocating a new vortex
};

#endif // STUDENTWORLD_H_

