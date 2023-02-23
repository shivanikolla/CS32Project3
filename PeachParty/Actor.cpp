#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void PlayerAvatar::doSomething()
{
    if (waitingToRollState())
    {
        if (getWorld()->getAction(playerNumber) == ACTION_ROLL) {
            
            int ticks = getTicks(); //fix this
            int dieroll = getDieRoll();
            
            dieroll = randInt(1, 10);
            ticks = dieroll*8;
            moveTo(getX()+2, getY());
        }
        else
            return;
    }
    else
    {
        
    }
   
}

void Peach::doSomething(){
    
    return;
}

void Yoshi::doSomething(){
    return;
}
