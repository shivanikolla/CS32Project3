#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <vector>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
////////////////////////////////////////////////////////////
///MovingObject Implementations //////
///////////////////////////////////////////////////////////
//
bool MovingObject::canMoveRight()
{
    if (!getWorld()->boardisempty(getX()+16, getY()))
        return true;
    
    return false;
}

bool MovingObject::canMoveLeft()
{
    if (!getWorld()->boardisempty(getX()-16, getY()))
        return true;
    
    return false;
}

bool MovingObject::canMoveUp()
{
    if (!getWorld()->boardisempty(getX(), getY()+16))
        return true;
    
    return false;
    
}

bool MovingObject::canMoveDown()
{
    if (!getWorld()->boardisempty(getX(), getY()-16))
        return true;
    
    return false;
}

int MovingObject::possibleMovementDirections() //function that will help determine if a character is at a fork
{
    int possibleDirections = 0;
    
    if (walk_direction == right)
    {
        if (canMoveRight()){
            possibleDirections++;
        }
        if (canMoveDown()) {
            possibleDirections++;
        }
        if (canMoveUp()) {
            possibleDirections++;
        }
    }
    else if (walk_direction == left)
    {
        if (canMoveLeft()) { //left
            possibleDirections++;
        }
        if (canMoveDown()) {//down
            possibleDirections++;
        }
        if (canMoveUp()) {//up
            possibleDirections++;
        }

    }
    else if (walk_direction == up)
    {
        if (canMoveUp()) {//up
            possibleDirections++;
        }
        if (canMoveLeft()) {//left
            possibleDirections++;
        }
        if (canMoveRight()) { //right
            possibleDirections++;
        }

    }
    else if (walk_direction == down)
    {
        if (canMoveDown()) {
            possibleDirections++;
        }
        if (canMoveRight()) {
            possibleDirections++;
        }
        if (canMoveLeft()) {
            possibleDirections++;
        }
    }

    return possibleDirections;

}

void MovingObject::characterAtTurningPoint() //if a character is at a turning point, this function adjusts the direction accordingly
{
    int nextX;
    int nextY;
    
    getPositionInThisDirection(getWalkDirection(), 16, nextX, nextY); // get the next square in the current direction
    
    switch (getWalkDirection()) {
        case right:
        case left:
         getPositionInThisDirection(up, 16, nextX, nextY); //get the position in the up direction
            
             if (!getWorld()->boardisempty(nextX, nextY)) //if it is not empty
                {
                    SetWalkDirection(up);
                    setDirection(0);
            }
            else
            {
                SetWalkDirection(down);
                setDirection(0);
            }
            break;
        case up:
        case down:
            getPositionInThisDirection(right, 16, nextX, nextY); //right takes priority over left, get position in right direction first
            
            if (!getWorld()->boardisempty(nextX, nextY)) //if it is not empty
            {
                SetWalkDirection(right);
                setDirection(0);
            }
            else
            {
                SetWalkDirection(left);
                setDirection(180);
            }
            break;
        default:
            break;
         }

}

//////////////////////////////////////////////////////
///Baddy Implementations///////////
////////////////////////////////////////////////////
void Baddy::chooseRandomDirection()
{
    std::vector<int> possibleDirections;

    if (canMoveRight()) {

        possibleDirections.push_back(0);
    }
    if (canMoveLeft()) {
        possibleDirections.push_back(180);
    }
    if (canMoveUp()) {
        possibleDirections.push_back(90);
    }
    if (canMoveDown()) {
        possibleDirections.push_back(270);
    }
    
    int randomIndex = rand()% possibleDirections.size();
    SetWalkDirection(possibleDirections[randomIndex]);
    
}

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
        else if (action == ACTION_FIRE && hasVortex == true) //if the player fires a vortex
        {
            int vortexX;
            int vortexY;
            
            int switchX;
            int switchY;
            
            getPositionInThisDirection(getWalkDirection(), 16, vortexX, vortexY);
            
            if (!getWorld()->boardisempty(vortexX, vortexY))
            {
                getWorld()->createVortex(vortexX, vortexY);
            }
            else if (getWalkDirection() == up)
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
            else if (getWalkDirection() == right)
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
//--------------------------------------------------------------------------------------------
   if (waitingToRoll == false) //otherwise, if it is in the WALKING state
   {
       
       int nextX;
       int nextY;
       
       getPositionInThisDirection(getWalkDirection(), 16, nextX, nextY); // get the next square in the current direction
       
       if (getX()%16 == 0 && getY()%16 == 0)  //only check if the current position is exactly on top of a square
       {
           if (getWorld()->isDirectionalSquare(getX(), getY())) { //checking if on a directional square
           
               if(getWorld()->isRightDirSquare(getX(), getY()))
               {
                   SetWalkDirection(right);
                   setDirection(0);
               }
           
               else if (getWorld()->isUpDirSquare(getX(), getY()))
               {
                   SetWalkDirection(up);
                   setDirection(0);
               }
               else if (getWorld()->isDownDirSquare(getX(), getY()))
               {
                   SetWalkDirection(down);
                   setDirection(0);
               }
               else if (getWorld()->isLeftDirSquare(getX(), getY()))
               {
                   SetWalkDirection(left);
                   setDirection(180);
               }
           }
       
           
           else if (possibleMovementDirections() >= 2 && previousState == false) //if possible directions is greater than or equal to 2, that means the object is at a fork and the previousState is not waitingToRoll
           {
               int action = getWorld()->getAction(playerNumber); //getting user input
               
               if (action == ACTION_RIGHT && canMoveRight() == true)
               {
                   SetWalkDirection(right);
                   setDirection(0);
               }
               else if (action == ACTION_LEFT && canMoveLeft()== true)
               {
                   SetWalkDirection(left);
                   setDirection(180);
               }
               else if (action == ACTION_DOWN && canMoveDown() == true)
               {
                   SetWalkDirection(down);
                   setDirection(0);
                   
               }
               else if (action == ACTION_UP && canMoveUp() == true)
               {
                   SetWalkDirection(up);
                   setDirection(0);
               }
               else
               {
                   return;
               }
               
           }
       
           else if (getWorld()->boardisempty(nextX, nextY)) //if it is empty, its at a turning point
           {
               characterAtTurningPoint();
           }
         
       }
       
       moveAtAngle(getWalkDirection(), 2);
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
    if (!stillAlive())
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
        
        if (getWorld()->getPeach()->newPlayerStatus()) {
        
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
    }
    
    else if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getYoshi()) || getWorld()->PlayerMovesOnSquare(this, getWorld()->getYoshi()))
    {
        if (getWorld()->getYoshi()->newPlayerStatus()) {
        
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
        getWorld()->getPeach()->setNewPlayerstatus(false);
        
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
        getWorld()->getYoshi()->setNewPlayerstatus(false);
        
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
            if (value == 1 && getHasActivatedOnPlayer() == false)
            {
                setHasActivatedOnPlayer(true);
                getWorld()->getPeach()->swapCoins(0);
                getWorld()->getPeach()->swapStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
            
        }
        else if (getWorld()->PlayersOnSameSquare(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->getWaitingToRollState() == true)
        {
            int value = randInt(1, 2);
            if (value == 1 && getHasActivatedOnPlayer() == false) {
                
                setHasActivatedOnPlayer(true);
                getWorld()->getYoshi()->swapCoins(0);
                getWorld()->getYoshi()->swapStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
        }
        
        setPauseCounter(-1);
        if (getpauseCounter() == 0)
        {
            int value = randInt(1, 10);
            setSquarestoMove(value);
            resetTickstoMove(8*value);
            
            chooseRandomDirection();
            setPausedState(false);
        }
        
    } //this brace closes of paused state if statement
    
    if (getPausedState() == false) //meaning Bowser is in the walking state
    {
        
        int nextX;
        int nextY;
        
        getPositionInThisDirection(getWalkDirection(), 16, nextX, nextY); // get the next square in the current direction
        
        if (getX()%16 == 0 && getY()%16 == 0) { //if Bowser is directly on top of a Square
            
            if (possibleMovementDirections() >=2) //if Bowser is at a fork
            {
                chooseRandomDirection();
                if (getWalkDirection() == left)
                    setDirection(180);
            }
            
            else if (getWorld()->boardisempty(nextX, nextY)) //if it is empty
            {
                characterAtTurningPoint();
            }
        }
        
        moveAtAngle(getWalkDirection(), 2);
        setTickstoMove(-1);
        if (getTickstoMove() == 0) {
            
            setHasActivatedOnPlayer(false);
            setPausedState(true);
            resetPauseCounter(180);
            int value = randInt(1, 4);
            if (value == 1) {
                
                getWorld()->createDroppingSquare(getX(), getY());
                getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                
            }
        }
        
    } //this brace closes of walking state if statement
    
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
            if (value == 1 && getHasActivatedOnPlayer() == false)
            {
                setHasActivatedOnPlayer(true);
                int Peachvalue = getWorld()->getPeach()->getCoins();
                int Yoshivalue = getWorld()->getYoshi()->getCoins();
                getWorld()->getPeach()->swapCoins(Yoshivalue);
                getWorld()->getYoshi()->swapCoins(Peachvalue);
                
            }
            else if (value == 2 && getHasActivatedOnPlayer() == false)
            {
                setHasActivatedOnPlayer(true);
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
          if (value == 1  && getHasActivatedOnPlayer() == false)
          {
              setHasActivatedOnPlayer(true);
              int Peachvalue = getWorld()->getPeach()->getCoins();
              int Yoshivalue = getWorld()->getYoshi()->getCoins();
              getWorld()->getPeach()->swapCoins(Yoshivalue);
              getWorld()->getYoshi()->swapCoins(Peachvalue);
              
          }
          else if (value == 2  && getHasActivatedOnPlayer() == false)
          {
              setHasActivatedOnPlayer(true);
              int PeachValue = getWorld()->getPeach()->getStars();
              int YoshiValue = getWorld()->getYoshi()->getStars();
              
              getWorld()->getPeach()->swapStars(YoshiValue);
              getWorld()->getYoshi()->swapStars(PeachValue);
              
          }
          
          getWorld()->playSound(SOUND_BOO_ACTIVATE);
          
      }
        
        setPauseCounter(-1);
        if (getpauseCounter() == 0)
        {
            int value = randInt(1, 3);
            setSquarestoMove(value);
            resetTickstoMove(8*value);

            chooseRandomDirection();
            setPausedState(false);
        }
   
    }
    
/////////////////////////////////////////////////
   if (!getPausedState())
   {
       if (getX()%16 == 0 && getY()%16 == 0) { //if Boo is directly on top of a square
       
       int nextX;
       int nextY;
       
       getPositionInThisDirection(getWalkDirection(), 16, nextX, nextY); // get the next square in the current direction
       
           if (possibleMovementDirections() >=2) //if Bowser is at a fork
           {
               chooseRandomDirection();
               if (getWalkDirection() == left)
                   setDirection(180);
           }
           
           else if (getWorld()->boardisempty(nextX, nextY)) //if it is empty
           {
               characterAtTurningPoint();
           }
    
           
       } //this brace closes checking if boo is directly on top of a square
       
       moveAtAngle(getWalkDirection(), 2);
       setTickstoMove(-1);
       if (getTickstoMove() == 0) {
           
           setHasActivatedOnPlayer(false);
           setPausedState(true);
           resetPauseCounter(180);
           
       }
       
       
       
   }// this brace closes off walking state

}

/////////////////////////////////////////////////////////
///EventSquare Implementations /////
/////////////////////////////////////////////////////////
void EventSquare::swapPositions(PlayerAvatar* a, PlayerAvatar* b)
{
    
    int tempAx = a->getX();
    int tempAy = a->getY();
    
    int tempBx = b->getX();
    int tempBy = b->getY();
    
    a->moveTo(tempBx, tempBy);
    b->moveTo(tempAx, tempAy);

    int tempTicks = a->returnTickstoMove();
    a->setTickstoMove(b->returnTickstoMove());
    b->setTickstoMove(tempTicks);
    
    int tempWalkDirection = a->WalkDirection();
    a->SetWalkDirection(b->getWalkDirection());
    b->SetWalkDirection(tempWalkDirection);
    
    int tempSpriteDirection = a->getDirection();
    a->setDirection(b->getDirection());
    b->setDirection(tempSpriteDirection);
    
    bool tempRollState = a->getWaitingToRollState();
    a->setWaitingToRollState(b->getWaitingToRollState());
    b->setWaitingToRollState(tempRollState);
    
}

void EventSquare::doSomething()
{
    
    if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getPeach()) && getWorld()->getPeach()->newPlayerStatus() == true)
    {
        int value = randInt(1, 3);
        switch (value) {
            case 1:
                getWorld()->getPeach()->setValidDirection(false);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case 2:
                
                swapPositions(getWorld()->getPeach(), getWorld()->getYoshi());
                
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
                
            case 3:
                if (!getWorld()->getPeach()->playerHasVortex()) {
                    getWorld()->getPeach()->setPlayerVortex(true);
                }
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
        }
        
    }
    else if (getWorld()->PlayerLandsOnSquare(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->newPlayerStatus() == true)
    {
        int value = randInt(1, 3);
        switch (value) {
            case 1:
                getWorld()->getYoshi()->setValidDirection(false);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case 2:
                
                swapPositions(getWorld()->getPeach(), getWorld()->getYoshi());
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case 3:
                if (!getWorld()->getYoshi()->playerHasVortex()) {
                    getWorld()->getYoshi()->setPlayerVortex(true);
                }
                
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
        }
        
    }
    
}

//////////////////////////////////////////////////////
///Vortex Implementations ////////////
/////////////////////////////////////////////////////
void Vortex::doSomething()
{
    ;
    
    
}
