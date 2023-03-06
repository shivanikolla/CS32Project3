#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Board.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject //Abstract base class that defines all actors
{
public:
    Actor(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): GraphObject(imageID, SPRITE_WIDTH*startX, SPRITE_HEIGHT*startY,dir, depth) {
        m_studentWorld = w;
        isAlive = true;
    }
    
    virtual void doSomething() = 0; //pure virtual function because we will never actually implement an actor object
    StudentWorld* getWorld() const {  return m_studentWorld; }
    virtual bool can_be_hit_by_vortex() const = 0;
    virtual bool is_a_square() const = 0;
    bool stillAlive() const {return isAlive;}
    void setAliveStatus(bool status) {isAlive= status;}
    
private:
    StudentWorld* m_studentWorld;
    bool isAlive;
    bool canBeHitByVortex;
};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Vortex : public Actor //Vortex doesn't share many similarities with any other classes so is directly derived from Actor
{
public:
    Vortex(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Actor(w, imageID, startX, startY, dir, depth) {
    }
    
    virtual void doSomething();
    virtual bool is_a_square() const { return false;}
    virtual bool can_be_hit_by_vortex() const {return false;}
    
private:

};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MovingObject: public Actor //movingObject class which PlayerAvatar, Boo, and Bowser are derived from
{
public:
    MovingObject(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Actor(w, imageID, startX, startY, dir, depth) {
        walk_direction = right;
        canGoUp = false;
        canGoDown = false;
        canGoRight = false;
        canGoLeft = false;
    }
    int possibleMovementDirections();
    int WalkDirection() {return walk_direction;}
    void SetWalkDirection(int value) {walk_direction = value;}
    int getWalkDirection() {return walk_direction;}
    bool canMoveRight();
    bool canMoveLeft();
    bool canMoveUp();
    bool canMoveDown();
    void characterAtTurningPoint();
    void characterAtFork();
    virtual void doSomething() {}
    virtual bool is_a_square() const { return false;}
    int returnTickstoMove() {return ticks_to_move;}
    void setTickstoMove(int value) {ticks_to_move +=value;}
    void resetTickstoMove(int value) {ticks_to_move = value;}
    virtual bool can_be_hit_by_vortex() const {return false;}
    
private:
    int walk_direction;
    bool canGoUp;
    bool canGoDown;
    bool canGoLeft;
    bool canGoRight;
    int ticks_to_move;
};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class PlayerAvatar: public MovingObject //playerAvatar class to represent Peach and Yoshi
{
public:
    PlayerAvatar(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth, int number): MovingObject(w, imageID, startX, startY, dir, depth) {
        
        playerNumber = number;
        die_roll = 0;
        hasVortex = false;
        m_Coins = m_Stars = 0;
        waitingToRoll = true;
        newPlayer = false;
        validDirection = true;
    }
    
    virtual void doSomething(); 
    int getCoins() const { return m_Coins; }
    int getStars() const { return m_Stars; }
    void setCoins(int coins) { m_Coins += coins;}
    void setStars(int stars) {m_Stars +=stars;}
    bool getState() const { return waitingToRoll;}
    void setNewPlayerstatus(bool value) {newPlayer = value;}
    bool newPlayerStatus() {return newPlayer; }
    bool getWaitingToRollState() {return waitingToRoll;}
    void setWaitingToRollState(bool value) { waitingToRoll = value;}
    void swapCoins(int value) {m_Coins = value;}
    void swapStars(int value) {m_Stars = value;}
    bool playerHasVortex() {return hasVortex;}
    void setPlayerVortex(bool value) {hasVortex = value;}
    bool getValidDirection()const {return validDirection;}
    void setValidDirection(bool value) {validDirection = value;}
    void swapPositions(PlayerAvatar* other);
    virtual bool can_be_hit_by_vortex() const {return false;}
    virtual bool is_a_square() const { return false;}
    
private:
    bool hasVortex;
    int m_Coins;
    int m_Stars;
    int playerNumber;
    int die_roll;
    bool waitingToRoll;
    bool newPlayer;
    bool validDirection;
    
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Baddy: public MovingObject
{
public:
    Baddy(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): MovingObject(w, imageID, startX, startY, dir, depth) {
        pausedState = true;
        pauseCounter = 180;
        initialTravelDistance = 0;
        hasActivatedOnPlayer = false;
    }
    virtual void doSomething();
    bool getPausedState() {return pausedState;}
    void setPausedState(bool value) { pausedState = value;}
    void setPauseCounter(int value) {pauseCounter += value;}
    void resetPauseCounter(int value) {pauseCounter = value;}
    int getpauseCounter() { return pauseCounter;}
    void setSquarestoMove(int value) {squares_to_move = value; }
    void chooseRandomDirection();
    bool getHasActivatedOnPlayer() {return hasActivatedOnPlayer;}
    void setHasActivatedOnPlayer(int value) {hasActivatedOnPlayer = value;}
    virtual bool is_a_square() const { return false;}
    virtual bool can_be_hit_by_vortex() const {return true;}
    void baddyInWalkingState();
    
private:
    bool pausedState; //I opted to create another paused state boolean for Baddy to avoid confusion with waitingtoRoll and walkingState of playerAvatar instead of creating a common one in MovingObject
    int pauseCounter;
    int initialTravelDistance;
    int squares_to_move;
    bool hasActivatedOnPlayer;
};

class Bowser : public Baddy
{
public:
    Bowser(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : Baddy(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething();
    virtual bool is_a_square() const { return false;}
    virtual bool can_be_hit_by_vortex() const {return true;}
private:
    
};

class Boo: public Baddy
{
public:
    Boo(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : Baddy(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething();
    virtual bool is_a_square() const { return false;}
    virtual bool can_be_hit_by_vortex() const {return true;}
private:
};

//------------------------------------------------------------------------------------------------------------------------------------------------
class Square: public Actor //Base class that all squares are derived from, useful in student world when determing whether players moved on to or landed on a square
{
public:
    virtual void doSomething() {return;}
    Square(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Actor(w, imageID, startX, startY, dir, depth) {
     
    }
    virtual bool is_a_square() const { return true;}
    virtual bool can_be_hit_by_vortex() const {return false;}
    
private:
  
};

class CoinSquare: public Square
{
public:
    CoinSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Square (w, imageID, startX, startY, dir, depth) {
    }
    virtual void doSomething();
    virtual bool can_be_hit_by_vortex() const {return false;}
    virtual bool is_a_square() const { return true;}
    
private:
    
};

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething();
    virtual bool can_be_hit_by_vortex() const {return false;}
    virtual bool is_a_square() const { return true;}
private:
    
};

class EventSquare : public Square
{
public:
    EventSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : Square(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething();
    virtual bool can_be_hit_by_vortex() const {return false;}
    void swapPositions(PlayerAvatar* a, PlayerAvatar* b);
    void teleportPlayer(PlayerAvatar* a);
    virtual bool is_a_square() const { return true;}
private:
};

class BankSquare : public Square
{
public:
    BankSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {
    }
    virtual void doSomething();
    virtual bool can_be_hit_by_vortex() const {return false;}
    virtual bool is_a_square() const { return true;}
private:
};

class DirectionSquare: public Square
{
public:
    DirectionSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Square(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {}
    virtual bool can_be_hit_by_vortex() const {return false;}
    virtual bool is_a_square() const { return true;}
private:
    
};

class DroppingSquare: public Square
{
public:
    DroppingSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Square(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething();
    virtual bool can_be_hit_by_vortex() const {return false;}
    virtual bool is_a_square() const { return true;}
private:
};

#endif // ACTOR_H_

