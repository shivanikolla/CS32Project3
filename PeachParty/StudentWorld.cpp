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
    
}

int StudentWorld::init()
{
    moneyInBank = 0;
    
    Board bd;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber())+ ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find board" << to_string(getBoardNumber()) <<".txt data file\n";
    else if (result == Board::load_fail_bad_format)
        cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success)
        cerr << "Successfully loaded board\n";
        
    for (int x = 0; x < BOARD_HEIGHT; x++) {
        for (int y = 0; y < BOARD_WIDTH; y++) {
            
            Board::GridEntry ge = bd.getContentsOf(x, y);
            
            switch (ge) {
                    
                case Board::empty: //populate with objects
                    cout << "Location" << x << "," << y << "is empty\n";
                    break;
                case Board::boo:
                    cout << "Location" << x << "," <<y << "has a Boo and a blue coin square\n";
                    m_actor.push_back(new Boo(this, IID_BOO, x, y, 0, 0 ));
                    break;
                case Board::bowser:
                    cout << "Location" << x << "," <<y << "has a Bowser and a blue coin square\n";
                    m_actor.push_back(new Bowser(this, IID_BOWSER, x, y, 0, 0));
                    break;
                case Board::player:
                    cout << "Location" << x << "," << y << "has a Player\n";
                    m_actor.push_back(new PlayerAvatar(this, IID_PEACH, x, y, 0, 0, 1));
                    m_actor.push_back(new PlayerAvatar(this, IID_YOSHI, x, y, 0, 0, 2));
                    break;
                case Board::red_coin_square:
                    cout << "Location" << x << "," << y << "has a red coin sqaure\n";
                    m_actor.push_back(new RedSquare(this, IID_RED_COIN_SQUARE, x, y, 0, 0));
                    break;
                case Board::blue_coin_square:
                    cout << "Location" << x << "," << y << "has a blue coin sqaure\n";
                    m_actor.push_back(new BlueSquare(this, IID_BLUE_COIN_SQUARE, x, y, 0, 0));
                    break;
                case Board::up_dir_square:
                    cout << "Location" << x << "," << y << "is a up direction sqaure\n";
                    m_actor.push_back(new UpDirectionSquare(this, IID_DIR_SQUARE, x, y, 90, 0));
                    break;
                case Board::down_dir_square:
                    cout << "Location" << x << "," << y << "is a down direction sqaure\n";
                    m_actor.push_back(new DownDirectionSquare(this, IID_DIR_SQUARE, x, y, 270, 0));
                    break;
                case Board::left_dir_square:
                    cout << "Location" << x << "," << y << "is a left direction sqaure\n";
                    m_actor.push_back(new UpDirectionSquare(this, IID_DIR_SQUARE, x, y, 180, 0));
                    break;
                case Board::right_dir_square:
                    cout << "Location" << x << "," << y << "is a right direction sqaure\n";
                    m_actor.push_back(new UpDirectionSquare(this, IID_DIR_SQUARE, x, y, 0, 0));
                    break;
                case Board::event_square:
                    cout << "Location" << x << "," << y << "is a right direction sqaure\n";
                    m_actor.push_back(new UpDirectionSquare(this, IID_EVENT_SQUARE, x, y, 0, 0));
                    break;
                case Board::bank_square:
                    cout << "Location" << x << "," << y << "is a right direction sqaure\n";
                    m_actor.push_back(new BankSquare(this, IID_BANK_SQUARE, x, y, 0, 0));
                    break;
                case Board::star_square:
                    cout << "Location" << x << "," << y << "is a right direction sqaure\n";
                    m_actor.push_back(new StarSquare(this, IID_STAR_SQUARE, x, y, 0, 0));
                    break;
            }
            
        }
    }
    
    
    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

 if (timeRemaining() <= 0) {
    playSound(SOUND_GAME_FINISHED);
 }
 
    
    
    
    setGameStatText("Game not implemented");
    
    if (timeRemaining() <= 0)
        return GWSTATUS_NOT_IMPLEMENTED;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    
    
}

