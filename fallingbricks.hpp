#ifndef _FALLINGBRICKS_H_ // include guards
#define _FALLINGBRICKS_H_

#include <cstdio> // used to print errors to the screen
#include <cstdlib> // used to pause the console if an error occurs. also used for the rand() function
#include <stdint.h> // contains definition of int8_t used for enums
#include <memory> // definitions for smart pointers
#include <sstream> // stringstream definition
#include <vector> // vector definition
#include <utility> // contains pair definition
#include <ctime> // used to seed the random number generator
#include <fstream> // file i/o operations
#include <algorithm> // operations for sorting vectors etc.

#include <sfml/graphics.hpp> // has all the basic graphics classes
#include <sfml/system.hpp> // has the clock definitions
#include <sfml/audio.hpp> // has the audio definitions

#include "piece.hpp" // custom piece class header
#include "board.hpp" // custom board class header


class FallingBricks // the class of the game itself
{
public:
    typedef std::pair<std::string, unsigned int> Score; // the structure used to hold each score object
public:
    static bool scoreCompare( Score a, Score b )
    {   // used for sorting the vector of scores by the second element. Order is highest to lowest
        return ( a.second > b.second );
    }
    FallingBricks(); // constructor. Takes a bool by reference which stores the success or failure of initialization
    FallingBricks( const FallingBricks& ); // default copy constructor
    ~FallingBricks(); // destructor. Nothing actually needs to be destroyed because of smart pointers

    void mainloop(); // the mainloop of the game. called by main()
private:
    typedef void (FallingBricks::*StateFuncPtr)(); // function pointer type.
    StateFuncPtr pfStateFunc; // instance of above typedef. Used to control game state
    void onMainMenu(); // runs once per frame if pfStateFunc points to it. Displays main menu
    void onGame(); // runs once per frame like above. This is the main game state
    void onPostGame(); // runs once per frame upon game completion. needs pfStateFunc to point to it like above

    void generateDefaultPieces(); // generates the first two pieces of the game
    void resetGame(); // resets the game to it's initial state
    void readScoresFromDisk(); // called during initialization. Reads high score data from hard disk.
    void saveScoresToDisk();   // called if player achieves a high score. saves data to hard disk.
    void updateScores(); // called to synchronize the main menu high score display with the actual list of high scores
    std::string encrypt(std::string); // encrypts the high score data (extremely simple)
    std::string decrypt(std::string); // complement of above

private:
    sf::RenderWindow mWindow; // the window of the application
    sf::Music mBackgroundMusic; // the ambient sound of the game
    sf::Music mBackgroundMusic2;
    std::unique_ptr<sf::SoundBuffer> mpCheerBuffer; // cheering sound played if user gets a high score
    std::unique_ptr<sf::SoundBuffer> mpNoHighScoreBuffer; // duh-duh-duh played if user does not get a high score
    std::unique_ptr<sf::SoundBuffer> mpOnLineClearedSoundBuffer; // played each time some lines are cleared
    sf::Sound mSound; // the sound manager
    std::vector<Score> mHighScores; // the high scores
    std::vector<sf::Text> mHighScoreDisplays; // the on screen text representation of the scores
    std::vector< std::pair<PieceType, sf::Texture> > mPieceTypes; // the types of pieces available in the game
    std::unique_ptr<Piece> mpPiece; // the piece that is actively falling
    std::unique_ptr<Piece> mpNextPiece; // the next piece in line
    std::shared_ptr<Board> mpBoard; // the virtual game board
    bool mShouldCount; // used to control the counter when the player scores
    bool mHighScoreObtained; // used to tell the post game state if it should gather the user's name or not
    bool mSystemRunning; // controls the main loop
    bool mPaused; // controls if the game is paused or not
    float mTime;  // the time since the last tick
    float mCounterTime; // the time since the last score increment (only if the user just scored)
    float mSpeed; // controls the speed of the game tick
    unsigned int mTickCount; // keeps track of tick counts for secondary sound play
    unsigned int mScore; // keeps the user's active score
    sf::Clock mCounterClock; // controls the score increment timing
    sf::Clock mClock; // controls the game tick timing
    sf::Texture mTitleTexture; // The texture for the game title on the main menu
    sf::Texture  mBGTexture; // the game board background texture
    sf::Texture mNextPieceBGTexture; // the texture behind the next piece
    sf::Texture mControlsTexture; // the texture that shows the player what controls to use
    sf::Event mEvent; // holds the keyboard events
    sf::Sprite mTitle; // sprite for the game title on main menu
    sf::Sprite mPlayArea; // sprite for the game board
    sf::Sprite mNextPiecePlaceholder; // sprite for the placeholder
    sf::Sprite mControls; // sprite for the controls texture
    sf::Font mFont; // the font object to draw text
    sf::Text mScoreboard; // shows the score
    sf::Text mLinesDisplay; // shows the line count
    sf::Text mMainMenuStart; // shows the "play" button
    sf::Text mMainMenuHighScores; // shows the high scores header
    sf::Text mMainMenuQuit; // shows the "quit" button
    sf::Text mPostGameMessage; // tells the user how they did after the game
    sf::Text mPostGamePrompt; // prompt for name after the game
    sf::Text mPostGameName; // the actual name the player types in
    sf::Text mPostGameOk; // the message that tells the user to press enter to continue
    sf::Text mPostGameCursor; // holds a blinking cursor for when the player enters their name
};

#endif
