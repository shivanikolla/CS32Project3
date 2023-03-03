#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////
///PlayerAvatar Implementations ////////
////////////////////////////////////////////////////////////

void PlayerAvatar::doSomething()
{
    bool previousState = false;
    if (waitingToRoll == true) { //if the playeravatar is in the waiting to roll state
        previousState = true;
        
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
       int possibleDirections = 0;
       bool canGoRight = false;
       bool canGoLeft= false;
       bool canGoUp = false;
       bool canGoDown = false;
       if (walk_direction == right) //checking if an object is at a fork
       {
           if (!getWorld()->boardisempty(getX()+16, getY())){
               possibleDirections++;
               canGoRight = true;
           }
           if (!getWorld()->boardisempty(getX(), getY()-16)) {
               possibleDirections++;
               canGoDown = true;
           }
           if (!getWorld()->boardisempty(getX(), getY()+16)) {
               possibleDirections++;
               canGoUp = true;
           }
       }
       else if (walk_direction == left)
       {
           if (!getWorld()->boardisempty(getX()-16, getY())) { //left
               possibleDirections++;
               canGoLeft = true;
           }
           if (!getWorld()->boardisempty(getX(), getY()-16)) {//down
               possibleDirections++;
               canGoDown = true;
           }
           if (!getWorld()->boardisempty(getX(), getY()+16)) {//up
               possibleDirections++;
               canGoUp = true;
           }
           
       }
       else if (walk_direction == up)
       {
           if (!getWorld()->boardisempty(getX(), getY()+16)) {//up
               possibleDirections++;
               canGoUp = true;
           }
           if (!getWorld()->boardisempty(getX()-16, getY())) {//left
               possibleDirections++;
               canGoLeft = true;
           }
           if (!getWorld()->boardisempty(getX()+16, getY())) { //right
               possibleDirections++;
               canGoRight = true;
           }

       }
       else if (walk_direction == down)
       {
           if (!getWorld()->boardisempty(getX(), getY()-16)) {
               possibleDirections++;
               canGoDown = true;
           }
           if (!getWorld()->boardisempty(getX()+16, getY())) {
               possibleDirections++;
               canGoRight = true;
           }
           if (!getWorld()->boardisempty(getX()-16, getY())) {
               possibleDirections++;
               canGoLeft = true;
           }
       }
       
       
       
       int nextX;
       int nextY;
       
       getPositionInThisDirection(walk_direction, 16, nextX, nextY); // get the next square in the current direction
       
       if (getX()%16 == 0 && getY()%16 == 0)  //only check if the current position is exactly on top of a square
       {
           if (getWorld()->isDirectionalSquare(getX(), getY())) { //checking if on a directional square
           
               if(getWorld()->isRightDirSquare(getX(), getY()))
               {
                   walk_direction = right;
               }
           
               else if (getWorld()->isUpDirSquare(getX(), getY()))
               {
                   walk_direction = up;
               }
               else if (getWorld()->isDownDirSquare(getX(), getY()))
               {
                   walk_direction = down;
               }
               else if (getWorld()->isLeftDirSquare(getX(), getY()))
               {
                   walk_direction = left;
               }
           }
       
           
           else if (possibleDirections >= 2 && previousState == false) //if possible directions is greater than or equal to 2, that means the object is at a fork
           {
               
               std::cerr << "entered fork statement";
               int action = getWorld()->getAction(playerNumber);
               
               if (action == ACTION_RIGHT && canGoRight == true)
               {
                   walk_direction = right;
                   setDirection(0);
               }
               else if (action == ACTION_LEFT && canGoLeft == true)
               {
                   walk_direction = left;
                   setDirection(180);
                   
               }
               else if (action == ACTION_DOWN && canGoDown == true)
               {
                   walk_direction = down;
                   setDirection(0);
                   
               }
               else if (action == ACTION_UP && canGoUp == true)
               {
                   walk_direction = up;
                   setDirection(0);
               }
               else
               {
                   return;
               }
               
           }
       
       else if (getWorld()->boardisempty(nextX, nextY)) //if it is empty
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
    if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getPeach()))   //checking if Peach has LANDED on the square
    {
        getWorld()->getPeach()->setNewPlayerstatus(false);
        
        int value = getWorld()->getBankAccountValue();
        getWorld()->getPeach()->setCoins(value);
        getWorld()->setBankAccountValue(-value);
        getWorld()->playSound(SOUND_WITHDRAW_BANK);


    }
    else if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getYoshi())) //checking if Yoshi has LANDED on the square
    {
        getWorld()->getYoshi()->setNewPlayerstatus(false);

        int value = getWorld()->getBankAccountValue();
        getWorld()->getYoshi()->setCoins(value);
        getWorld()->setBankAccountValue(-value);
        getWorld()->playSound(SOUND_WITHDRAW_BANK);

    }
    else if (getWorld()->PlayerMovesOnSquare(this, getWorld()->getPeach())) //checking if Peach has MOVED on the square
    {
        int value = getWorld()->getPeach()->getCoins();
        if (value < 5) {

            getWorld()->getPeach()->setCoins(-value);
            getWorld()->setBankAccountValue(value);
        }
        else
        {
            
            getWorld()->getPeach()->setCoins(-5);
            getWorld()->setBankAccountValue(5);
        }
        getWorld()->playSound(SOUND_DEPOSIT_BANK);

    }
    else if (getWorld()->PlayerMovesOnSquare(this, getWorld()->getYoshi())) //checking if Yoshi has MOVED on the square
    {

        int value = getWorld()->getYoshi()->getCoins();
        if (value < 5) {
            
            getWorld()->getYoshi()->setCoins(-value);
            getWorld()->setBankAccountValue(value);
        }
        else
        {
            getWorld()->getYoshi()->setCoins(-5);
            getWorld()->setBankAccountValue(5);
        }

        getWorld()->playSound(SOUND_DEPOSIT_BANK);

    }
//
}

/////////////////////////////////////////////////////////////////
///Dropping Square Implementations /////
////////////////////////////////////////////////////////////////

void DroppingSquare::doSomething()
{
    
    if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getPeach()) && getWorld()->getPeach()->newPlayerStatus() == true)
    {
        int value = randInt(1, 2);
        int playerCoins = getWorld()->getPeach()->getCoins();
        int playerStars = getWorld()->getPeach()->getStars();
        switch (value) {
            case 1:
                if (playerCoins < 10) {
                    
                    getWorld()->getPeach()->setCoins(-playerCoins);
                    
                }
                else
                {
                    getWorld()->getPeach()->setCoins(-10);
                }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
            case 2:
               if (playerStars >=1)
               {
                   getWorld()->getPeach()->setStars(-1);
               }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
        }
    }
    else if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->newPlayerStatus() == true)
    {
        
        int value = randInt(1, 2);
        int playerCoins = getWorld()->getYoshi()->getCoins();
        int playerStars = getWorld()->getYoshi()->getStars();
        
        switch (value) {
            case 1:
                if (playerCoins < 10) {
                    
                    getWorld()->getYoshi()->setCoins(-playerCoins);
                    
                }
                else
                {
                    getWorld()->getYoshi()->setCoins(-10);
                }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
            case 2:
                if (playerStars >=1)
                {
                    getWorld()->getYoshi()->setStars(-1);
                }
                getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
                break;
        }
        
    }
  
}


///////////////////////////////////////////////
///Bowser Implementations ////
///////////////////////////////////////////////

void Bowser::doSomething()
{
    if(getPausedState())
    {
        
        if (getWorld()->PlayersOnSameSquare(this, getWorld()->getPeach()) && getWorld()->getPeach()->getWaitingToRollState() == true)
        {
            int value = randInt(1, 2);
            if (value == 1)
            {
                getWorld()->getPeach()->swapCoins(0);
                getWorld()->getPeach()->swapStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
            
        }
        else if (getWorld()->PlayersOnSameSquare(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->getWaitingToRollState() == true)
        {
            int value = randInt(1, 2);
            if (value == 1)
            {
                getWorld()->getYoshi()->swapCoins(0);
                getWorld()->getYoshi()->swapStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
 
        }
        
        
        setPauseCounter(-1);
        
        
    }
    
    
    
    
    
    
    
}

/////////////////////////////////////////////
///Boo Implementations ////////
/////////////////////////////////////////////

void Boo::doSomething()
{
    if (getPausedState())
    {
        if (getWorld()->PlayersOnSameSquare(this, getWorld()->getPeach()) && getWorld()->getPeach()->getWaitingToRollState() == true) //checking if Peach and Boo are on the same square first
        {
            int value = randInt(1, 2);
            if (value == 1)
            {
                int Peachvalue = getWorld()->getPeach()->getCoins();
                int Yoshivalue = getWorld()->getYoshi()->getCoins();
                getWorld()->getPeach()->swapCoins(Yoshivalue);
                getWorld()->getYoshi()->swapCoins(Peachvalue);
                
            }
            else if (value == 2)
            {
                int PeachValue = getWorld()->getPeach()->getStars();
                int YoshiValue = getWorld()->getYoshi()->getStars();
                
                getWorld()->getPeach()->swapStars(YoshiValue);
                getWorld()->getYoshi()->swapStars(PeachValue);
                
            }
            
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
        }
      else if (getWorld()->PlayersOnSameSquare(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->getWaitingToRollState() == true)
      {
          
          int value = randInt(1, 2);
          if (value == 1)
          {
              int Peachvalue = getWorld()->getPeach()->getCoins();
              int Yoshivalue = getWorld()->getYoshi()->getCoins();
              getWorld()->getPeach()->swapCoins(Yoshivalue);
              getWorld()->getYoshi()->swapCoins(Peachvalue);
              
          }
          else if (value == 2)
          {
              int PeachValue = getWorld()->getPeach()->getStars();
              int YoshiValue = getWorld()->getYoshi()->getStars();
              
              getWorld()->getPeach()->swapStars(YoshiValue);
              getWorld()->getYoshi()->swapStars(PeachValue);
              
          }
          
          getWorld()->playSound(SOUND_BOO_ACTIVATE);
          
      }
        
        setPauseCounter(-1);
        setPausedState(false);
    }
   if (!getPausedState())
   {
       
       
       
   }
    
}

/////////////////////////////////////////////////////////
///EventSquare Implementations /////
/////////////////////////////////////////////////////////
void EventSquare::doSomething()
{
    
    if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getPeach()) && getWorld()->getPeach()->newPlayerStatus() == true)
    {
        int value = randInt(1, 3);
        switch (value) {
            case 1:
                break;
            case 2:
                break;
                
            case 3:
                break;
        }
        
        
        
    }
    
    
    
}
