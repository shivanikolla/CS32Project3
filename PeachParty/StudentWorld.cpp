#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
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
    newVortex = nullptr;
    bankAccountValue = 0;
    newDroppingSquare = nullptr;
    
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
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
 // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    setGameStatText("P1 Roll: " + to_string(p->getStars()) + " Stars: " + to_string(p->getCoins()) + " $$ | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getBankAccountValue())+ " | P2 Roll: " + to_string(yoshi->getStars()) + " Stars: " + to_string(yoshi->getCoins()) + " $$");
    
    if (timeRemaining() <= 0) {
        
       playSound(SOUND_GAME_FINISHED);
        
        int peachSum = p->getCoins() + p->getStars();
        int yoshiSum = yoshi->getCoins() + yoshi->getStars();
        
        if (peachSum > yoshiSum) {
            
            setFinalScore(p->getStars(), p->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (yoshiSum > peachSum) {
            
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

    for (int i = 0; i <m_actor.size(); i++){
        
        if (m_actor[i]->stillAlive() == true) {
            m_actor[i]->doSomething();
            
        }
        else
        {
            delete m_actor[i];
            m_actor.clear();
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

bool StudentWorld::hasPlayer(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::player)
    {
        return true;
    }
    return false;
}

void StudentWorld::createVortex(int x, int y)
{
    newVortex = new Vortex(this, IID_VORTEX, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, 0, 0);
    m_actor.push_back(newVortex);
}

void StudentWorld::createDroppingSquare(int x, int y)
{
    newDroppingSquare = new DroppingSquare(this, IID_DROPPING_SQUARE, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, 0, 0);
    m_actor.push_back(newDroppingSquare);
    
}

bool StudentWorld::isUpDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::up_dir_square)
    {
        return true;
    }
    
    return false;
}

bool StudentWorld::isDownDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::down_dir_square)
    {
        return true;
    }
    
    return false;
    
}

bool StudentWorld::isRightDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::right_dir_square)
    {
        return true;
    }
    
    return false;
}

bool StudentWorld::isLeftDirSquare(int x, int y)
{
    if (b->getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::left_dir_square)
    {
        return true;
    }
    
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
