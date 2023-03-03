#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////
///PlayerAvatar Implementations ////////
////////////////////////////////////////////////////////////

void PlayerAvatar::doSomething()
{
    if (waitingToRoll == true) { //if the playeravatar is in the waiting to roll state
        
        int action = getWorld()-> getAction(playerNumber);
        if (action == ACTION_ROLL) { //rolling the "dice"
            die_roll = randInt(1, 10);
            ticks_to_Move = die_roll*8;
            waitingToRoll = false;
        }
        else if (action == ACTION_FIRE) //if the player fires a vortex
        {
            int vortexX;
            int vortexY;
            
            int switchX;
            int switchY;
            
            getPositionInThisDirection(walk_direction, 16, vortexX, vortexY);
            
            if (!getWorld()->boardisempty(vortexX, vortexY))
            {
                getWorld()->createVortex(vortexX, vortexY);
            }
            else if (walk_direction == up)
            {
                getPositionInThisDirection(left, 16, switchX, switchY);
                
                if (!getWorld()->boardisempty(switchX, switchY))
                {
                    getWorld()->createVortex(switchX, switchY);
                }
                else
                {
                    getWorld()->createVortex(getX()+16, getY());
                }
            }
            else if (walk_direction == right)
            {
                getPositionInThisDirection(up, 16, switchX, switchY);
                if (!getWorld()->boardisempty(switchX, switchY))
                {
                    getWorld()->createVortex(switchX, switchY);
                }
            }

            getWorld()->playSound(SOUND_PLAYER_FIRE);
            hasVortex = false;
            
        }
        else { return; }
    }

   if (waitingToRoll == false) //otherwise, if it is in the WALKING state
   {
       if (getX()%16 == 0 && getY()% 16 == 0) {
       
       if(getWorld()->isRightDirSquare(getX(), getY())) {
           walk_direction = right;
       }
       else if (getWorld()->isUpDirSquare(getX(), getY())) {
           walk_direction = up;
       }
       else if (getWorld()->isDownDirSquare(getX(), getY())) {
           walk_direction = down;
       }
       else if (getWorld()->isLeftDirSquare(getX(), getY())) {
           walk_direction = left;
       }
       
       }
       int nextX;
       int nextY;
       
       if (getX()%16 == 0 && getY()%16 == 0) {//only check if the current position is exactly on top of a square
           
           getPositionInThisDirection(walk_direction, 16, nextX, nextY); // get the next square in the current direction
       
       
       if (getWorld()->boardisempty(nextX, nextY)) //if it is empty
       {
           switch (walk_direction) {
               case right:
               case left:
                getPositionInThisDirection(up, 16, nextX, nextY); //get the position in the up direction
                   
                    if (!getWorld()->boardisempty(nextX, nextY)) //if it is not empty
                       {
                           walk_direction = up; //change the walk direction to up
                           setDirection(0);
                   }
                   else
                   {
                       walk_direction = down; //otherwise change it to down
                       setDirection(0);
                   }
                   break;
               case up:
               case down:
                   getPositionInThisDirection(right, 16, nextX, nextY); //right takes priority over left, get position in right direction first
                   
                   if (!getWorld()->boardisempty(nextX, nextY)) //if it is not empty
                   {
                       walk_direction = right; //set the direction to right
                       setDirection(0);
                   }
                   else
                   {
                       walk_direction = left;
                       setDirection(180);
                   }
                   break;
               default:
                   break;
           }
           
            }
         
       }
       
       moveAtAngle(walk_direction, 2);
       ticks_to_Move --;
       if (ticks_to_Move == 0) {
           waitingToRoll = true;
           newPlayer = true;
       }
   }
}

///////////////////////////////////////////////////////
///CoinSquare Implementations ////
//////////////////////////////////////////////////////

void CoinSquare::doSomething()
{
    if (!isSquareActive())
        {return;}
  
    if (getX()%16 == 0 && getY()%16 == 0){
    
    if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getPeach()) && getWorld()->getPeach()->newPlayerStatus() == true) //checking if Peach has landed on the square
    {
        getWorld()->getPeach()->setNewPlayerstatus(false);
        
        if (getWorld()->isBlueCoinSquare(getX(), getY())){
            
        getWorld()->getPeach()->setCoins(3);
        getWorld()->playSound(SOUND_GIVE_COIN);
            
        
        }
        else if (getWorld()->isRedCoinSquare(getX(), getY())) {
            
            int value = getWorld()->getPeach()->getCoins();
            if (value < 3) {
                getWorld()->getPeach()->setCoins(-value);
            }
            else
            {
                getWorld()->getPeach()->setCoins(-3);
            }
            getWorld()->playSound(SOUND_TAKE_COIN);
        }
    }
    
    else if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->newPlayerStatus() == true) //checking if Yoshi has landed on the square
    {
        getWorld()->getYoshi()->setNewPlayerstatus(false);
        
        if (getWorld()->isBlueCoinSquare(getX(), getY())) {
            
            getWorld()->getYoshi()->setCoins(3);
            getWorld()->playSound(SOUND_GIVE_COIN);
        }
        else if (getWorld()->isRedCoinSquare(getX(), getY())) {
            
            int value = getWorld()->getYoshi()->getCoins();
            if (value < 3) {
                
                getWorld()->getYoshi()->setCoins(-value);
            }
            else
            {
                getWorld()->getYoshi()->setCoins(-3);
            }
            getWorld()->playSound(SOUND_TAKE_COIN);
        }
    }
}

}

///////////////////////////////////////////////////////////
///StarSquare Implementations ///////
/////////////////////////////////////////////////////////

void StarSquare::doSomething()
{
    
    //checking Peach first
    if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getPeach()) || getWorld()->PlayerMovesOnSquare(this, getWorld()->getPeach())) {
        
        getWorld()->getPeach()->setNewPlayerstatus(false);
        
        if (getWorld()->getPeach()->getCoins() < 20)
        {
            return;
        }
        else
        {
            getWorld()->getPeach()->setCoins(-20);
            getWorld()->getPeach()->setStars(1);
            getWorld()->playSound(SOUND_GIVE_STAR);
            
        }
    }
    
    else if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getYoshi()) || getWorld()->PlayerMovesOnSquare(this, getWorld()->getYoshi()))
    {
        getWorld()->getYoshi()->setNewPlayerstatus(false);
        
        if (getWorld()->getYoshi()->getCoins() < 20)
        {
            return;
        }
        else
        {
            getWorld()->getYoshi()->setCoins(-20);
            getWorld()->getYoshi()->setStars(1);
            getWorld()->playSound(SOUND_GIVE_STAR);
            
        }
    }
  
}

///////////////////////////////////////////////////////
///BankSquare Implementations/////
////////////////////////////////////////////////////////

void BankSquare::doSomething()
{
    
//    //checking Peach first
//    if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getPeach())
//        {
//
//
//    }
    
    
    
    
    
    
    
}
