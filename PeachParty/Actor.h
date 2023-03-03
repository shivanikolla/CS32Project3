#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Board.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject //Abstract that base class that defines all actors
{
public:
    Actor(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): GraphObject(imageID, SPRITE_WIDTH*startX, SPRITE_HEIGHT*startY,dir, depth) {
        m_studentWorld = w;
        isAlive = true;
    }
    
    virtual void doSomething() = 0; //pure virtual function because we will never actually implement an actor object
    StudentWorld* getWorld() const {  return m_studentWorld; }
//    virtual bool is_a_square() const = 0;
//    virtual bool can_be_hit_by_vortex() const;
//    bool is_active() const;
    
    
    bool stillAlive() const {return isAlive;}
    void setAliveStatus(bool status) {isAlive= status;}
    
    
private:
    StudentWorld* m_studentWorld;
    bool isAlive;
};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Vortex : public Actor //Vortex doesn't share many similarities with any other classes so is directly derived from Actor
{
public:
    Vortex(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Actor(w, imageID, startX, startY, dir, depth) {
        isActive = true;
    }
    
    virtual void doSomething(){}
    
private:
    bool isActive;

};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class PlayerAvatar: public Actor //playerAvatar class to represent Peach and Yoshi because they behave similarly
{
public:
    PlayerAvatar(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth, int number): Actor(w, imageID, startX, startY, dir, depth) {
        
        playerNumber = number;
        walk_direction = right;
        die_roll = 0;
        hasVortex = false;
        ticks_to_Move = 0;
        m_Coins = m_Stars = 0;
        waitingToRoll = true;
        newPlayer = false;
    }
    
    virtual void doSomething(); 
    int getCoins() const { return m_Coins; }
    int getStars() const { return m_Stars; }
    int getWalkDirection() const { return walk_direction; }
    void setCoins(int coins) { m_Coins += coins;}
    void setStars(int stars) {m_Stars +=stars;}
    bool getState() const { return waitingToRoll;}
    void setNewPlayerstatus(bool value) {newPlayer = value;}
    bool newPlayerStatus() {return newPlayer; }
    
private:
    int ticks_to_Move;
    bool hasVortex;
    int m_Coins;
    int m_Stars;
    int playerNumber;
    int die_roll;
    int walk_direction;
    bool waitingToRoll;
    bool newPlayer;
    Vortex* newVortex;
    
};

//------------------------------------------------------------------------------------------------------------------------------------------------
class Square: public Actor //Base class that all squares are derived from
{
public:
    virtual void doSomething() {return;}
    Square(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Actor(w, imageID, startX, startY, dir, depth) {
        isActive = true;
    }

    bool isSquareActive() {return isActive;}
    
private:
    bool isActive;
};

class CoinSquare: public Square
{
public:
    CoinSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Square (w, imageID, startX, startY, dir, depth) {
    }
    virtual void doSomething();
    
private:
    
};

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething();
private:
    
};

class EventSquare : public Square
{
public:
    EventSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth) : Square(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething(){}

private:
};

class BankSquare : public Square
{
public:
    BankSquare(StudentWorld * w, int imageID, int startX, int startY, int dir, int depth) :  Square(w, imageID, startX, startY, dir, depth) {
        bankAccountValue = 0;
    }
    
    int getBankAccountValue() const { return bankAccountValue;}
    void setBankAccountValue(int value) { bankAccountValue += value;}
    virtual void doSomething();
private:
    int bankAccountValue;
};

class DirectionSquare: public Square
{
public:
    DirectionSquare(StudentWorld* w, int imageID, int startX, int startY, int dir, int depth): Square(w, imageID, startX, startY, dir, depth) {}
    virtual void doSomething() {}
    
private:
    
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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


#endif // ACTOR_H_

