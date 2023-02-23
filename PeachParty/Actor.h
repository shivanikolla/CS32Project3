#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject //add in a pointer to the student world object its playing in
{
public:
    Actor(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): GraphObject(imageID, SPRITE_WIDTH*startX, SPRITE_HEIGHT*startY,dir, depth) {
        
        m_studentWorld = w;
        waitingToRoll = true;
        alive = true;
        
    }
    
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const {  return m_studentWorld; }
    bool waitingToRollState() const {  return waitingToRoll; }

private:
    StudentWorld* m_studentWorld;
    bool waitingToRoll;
    bool alive;
    
};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

class PlayerAvatar: public Actor
{
public:
    PlayerAvatar(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth, int number): Actor(w, imageID, startX, startY, dir, depth) {
        
        playerNumber = number;
        walk_direction = 0;
        die_roll = 0;
        hasVortex = false;
        ticks_to_Move = 0;
        
    }
    virtual void doSomething();//call determinePlayerNumber to decide if it needs to use left or right side of keyboard
    int getTicks() const {  return ticks_to_Move;  }
    int getDieRoll() const { return die_roll; }
    
private:
    int ticks_to_Move;
    bool hasVortex;
    int coins;
    int stars;
    int playerNumber;
    int die_roll;
    int walk_direction;
    
};

class Peach : public PlayerAvatar
{
public:
    Peach(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth, int number): PlayerAvatar(w, imageID, startX, startY, dir ,depth, number) {
      
    }
    virtual void doSomething();
private:
};

class Yoshi : public PlayerAvatar
{
public:
    Yoshi(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth, int number): PlayerAvatar(w, imageID, startX, startY, dir ,depth, number) {
     
    }
    virtual void doSomething();
private:
};

//------------------------------------------------------------------------------------------------------------------------------------------------

class Baddy: public Actor
{
public:
    Baddy(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Actor(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {return;}
private:
    
};

class Bowser : public Baddy
{
public:
    Bowser(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : Baddy(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {}
private:
    
};

class Boo: public Baddy
{
public:
    Boo(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : Baddy(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {}
private:
};


//------------------------------------------------------------------------------------------------------------------------------------------------
class Square: public Actor //Abstract Base class that all squares are derived from
{
public:
//    virtual void changePlayersStarsorCoins();
    virtual void doSomething() {return;}
    Square(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Actor(w, imageID, startX, startY, dir, depth) {}
};

class CoinSquare: public Square
{
public:
    CoinSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Square (w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {}
private:
};

class BlueSquare: public CoinSquare
{
public:
    BlueSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : CoinSquare(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {}
private:
};

class RedSquare: public CoinSquare
{
public:
    RedSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : CoinSquare(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {}
private:
    
    
};

class EventSquare : public Square
{
public:
    EventSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : Square(w, imageID, startX, startY, dir, depth) {}

private:
};

class BankSquare : public Square
{
public:
    BankSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {}
   
private:

};

class UpDirectionSquare : public Square
{
public:
    UpDirectionSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {}
    
private:
    
};

class DownDirectionSquare: public Square
{
public:
    DownDirectionSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {}
    
private:
    
};

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {}
private:
    
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Vortex : public Actor
{
public:
    virtual void doSomething();
    void vortexOverlapsWithaBaddie();

private:
    bool isActive = true;

};

#endif // ACTOR_H_

