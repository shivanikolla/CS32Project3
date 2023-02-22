#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void PlayerAvatar::doSomething()
{
    return;
}

void Peach::doSomething(){
    
    if (waitingToRollState())
    {
        if (getWorld()->getAction(1) == ACTION_ROLL) {
            
            
        }
        else
            return;
    }
    
}

void Yoshi::doSomething() {
    
    ;
}
