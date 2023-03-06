#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    p = nullptr;
    yoshi = nullptr;
    b = new Board();
    bankAccountValue = 0;
    
}

int StudentWorld::init()
{    
    string board_file = assetPath() + "board0" + to_string(getBoardNumber())+ ".txt";
    Board::LoadResult result = b->loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board" << to_string(getBoardNumber()) <<".txt data file\n";
        return GWSTATUS_BOARD_ERROR; }
    else if (result == Board::load_fail_bad_format)
        cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success)
        cerr << "Successfully loaded board\n";
        
    for (int x = 0; x < BOARD_HEIGHT; x++) {
        for (int y = 0; y < BOARD_WIDTH; y++) {
            
            Board::GridEntry ge = b->getContentsOf(x, y);
            
            switch (ge) {
                    
                case Board::empty: //populate with objects
                    break;
                case Board::boo:
                    m_actor.push_back(new Boo(this, IID_BOO, x, y, 0, 0 ));
                    m_actor.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x, y, 0, 1));
                    break;
                case Board::bowser:
                    m_actor.push_back(new Bowser(this, IID_BOWSER, x, y, 0, 0));
                    m_actor.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x, y, 0, 1));
                    break;
                case Board::player:
                    m_actor.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x, y, 0, 1));
                    p = new PlayerAvatar(this, IID_PEACH, x, y, 0, 0, 1);
                    yoshi = new PlayerAvatar(this, IID_YOSHI, x, y, 0, 0, 2);
                    break;
                case Board::red_coin_square:
                    m_actor.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, x, y, 0, 1));
                    break;
                case Board::blue_coin_square:
                    m_actor.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x, y, 0, 1));
                    break;
                case Board::up_dir_square:
                    m_actor.push_back(new DirectionSquare(this, IID_DIR_SQUARE, x, y, 90, 1));
                    break;
                case Board::down_dir_square:
                    m_actor.push_back(new DirectionSquare(this, IID_DIR_SQUARE, x, y, 270, 1));
                    break;
                case Board::left_dir_square:
                    m_actor.push_back(new DirectionSquare(this, IID_DIR_SQUARE, x, y, 180, 1));
                    break;
                case Board::right_dir_square:
                    m_actor.push_back(new DirectionSquare(this, IID_DIR_SQUARE, x, y, 0, 1));
                    break;
                case Board::event_square:
                    m_actor.push_back(new EventSquare(this, IID_EVENT_SQUARE, x, y, 0, 1));
                    break;
                case Board::bank_square:
                    m_actor.push_back(new BankSquare(this, IID_BANK_SQUARE, x, y, 0, 1));
                    break;
                case Board::star_square:
                    m_actor.push_back(new StarSquare(this, IID_STAR_SQUARE, x, y, 0, 1));
                    break;
                    
            }
        }
    }
    
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    ostringstream oss;
    
    oss.fill('0');
    oss << "P1 Roll: ";
    oss << p->returnTickstoMove()/8 << " ";
    oss << "Stars: ";
    oss << p->getStars() << " ";
    oss << "$$: ";
    oss << p->getCoins() << " |";
    if (p->playerHasVortex()) {
        oss << " VOR";
    }
    oss << " Time: ";
    oss << timeRemaining() << " |";
    oss << " Bank: ";
    oss << getBankAccountValue() << " |";
    oss << " P2 Roll: ";
    oss<< yoshi->returnTickstoMove()/8 << " ";
    oss << "Stars: ";
    oss << yoshi->getStars() << " ";
    oss << "$$: ";
    oss << yoshi->getCoins();
    if (yoshi->playerHasVortex()) {
        oss << " VOR ";
    }
    
    setGameStatText(oss.str());
    
    if (timeRemaining() <= 0) {
        
       playSound(SOUND_GAME_FINISHED);
        
        if (p->getStars() > yoshi->getStars()) {
            
            setFinalScore(p->getStars(), p->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (yoshi->getStars() > p->getStars()) {
            
            setFinalScore(yoshi->getStars(), yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else if (p->getCoins() > yoshi->getCoins()) {
            
            setFinalScore(p->getStars(), p->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (yoshi->getCoins() > p->getCoins()) {
            
            setFinalScore(yoshi->getStars(), yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
            
        }
        else
        {
            int number = randInt(1, 2);
            if (number == 1)
            {
                setFinalScore(p->getStars(), p->getCoins());
                return GWSTATUS_PEACH_WON;
            }
            else {
                setFinalScore(yoshi->getStars(), yoshi->getCoins());
                return GWSTATUS_YOSHI_WON;
            }
        }
    }
    
    if (yoshi->stillAlive() && p->stillAlive())
    {
        yoshi->doSomething();
        p->doSomething();
    }
    
    for (vector<Actor*>::iterator it = m_actor.begin(); it != m_actor.end(); it++) {
        
        if ((*it)->stillAlive()) {
            (*it)->doSomething();
        }
        else if ((*it)->stillAlive() == false)
        {
            delete (*it);
            m_actor.erase(it);
            it = m_actor.begin();
        }
        
    }
    
    return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::cleanUp()
{
     
    for (Actor* p : m_actor)
        delete p;
    m_actor.clear();
    
    delete p;
    delete yoshi;
    p = yoshi = nullptr;
    
    delete b;
    b = nullptr;
    
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

/////////////////////////////////////////////
///////helper functions //////////
////////////////////////////////////////////

bool StudentWorld::boardisempty(int x, int y)
{
   if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::empty) {
       return true;
   }

    return false;
}


bool StudentWorld::isBlueCoinSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::blue_coin_square) {
        return true;
    }

    return false;
}

bool StudentWorld::isRedCoinSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) ==Board::red_coin_square) {
        return true;
    }
    
    return false;
}

void StudentWorld::createVortex(int x, int y, int walkDirection)
{
    
    m_actor.push_back(new Vortex(this, IID_VORTEX, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, 0, 0));
    
    vortexWalkDirection = walkDirection;
}

void StudentWorld::createDroppingSquare(int x, int y)
{
    for (vector<Actor*>::iterator it = m_actor.begin(); it != m_actor.end(); it++) {
        
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->is_a_square()) {
            
            delete (*it);
            m_actor.erase(it);
            it = m_actor.begin();
        }
    }
    
    m_actor.push_back( new DroppingSquare(this, IID_DROPPING_SQUARE, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, 0, 1));
    
}

bool StudentWorld::isUpDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::up_dir_square)
        return true;
    
    return false;
}

bool StudentWorld::isDownDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::down_dir_square)
        return true;
    
    return false;
    
}

bool StudentWorld::isRightDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::right_dir_square)
        return true;
    
    return false;
}

bool StudentWorld::isLeftDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::left_dir_square)
        return true;
    
    return false;
}

bool StudentWorld::isDirectionalSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::left_dir_square || b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::right_dir_square|| b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::down_dir_square || b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::up_dir_square)
    {
        return true;
    }
    
    return false;
    
}

bool StudentWorld::PlayerLandsOnSquare(Square* a, PlayerAvatar* b)
{
    if (a->getX() == b->getX() && a->getY() == b->getY())
    {
        if (p->getState() == true) //means the player is in the waiting to roll state
        return true;
    }
    return false;
}

bool StudentWorld::PlayerMovesOnSquare(Square* a, PlayerAvatar* b)
{
    if (a->getX() == b->getX() && a->getY() == b->getY())
    {
        if (p->getState() == false) //means the player is in the walking state
            return true;
    }
    return false;
    
}

bool StudentWorld::PlayersOnSameSquare(Actor* a, Actor* b)
{
    if (a->getX() == b->getX() && a->getY() == b->getY())
    {
        return true;
    }
    return false;
}


void StudentWorld::randomCoordinateGenerator() //a recursive random coordinate number generator
{
    int x = randInt(0, 255);
    int y = randInt(0, 255);
    if (x%16 == 0 && y%16 == 0) {
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) != Board::empty) {
        randomX = x;
        randomY = y;
        return;
        }
    }
    else
        randomCoordinateGenerator();
    
}

void StudentWorld::teleportBaddy(Actor* b)
{
    randomCoordinateGenerator(); //generates a random set of coordinates on the board
    
    if (getRandomX()%16 == 0 && getRandomY()%16 == 0) { //checks that they are exactly on top of a square
    
        if (!boardisempty(getRandomX(), getRandomY())) //if that square is not empty
        {
            b->moveTo(getRandomX(), getRandomY()); //then move the baddy to that square
        }
    }
    BaddyHasBeenTeleported = true;
}

void StudentWorld::objectOverlapwithVortex(Vortex* v)
{
    for (vector<Actor*>::iterator it = m_actor.begin(); it != m_actor.end(); it++)
    {
        if ((*it)->can_be_hit_by_vortex())
        {
            if (v->getX() + SPRITE_WIDTH > (*it)->getX() &&  v->getX() < (*it)->getX()+SPRITE_WIDTH)
            {
                if (v->getY() + SPRITE_HEIGHT > (*it)->getY() && v->getY() < (*it)->getY()+SPRITE_HEIGHT)
                {
                    teleportBaddy((*it));
                    playSound(SOUND_HIT_BY_VORTEX);
                    break;
                    
                }
            }
            
        }
    }
}

