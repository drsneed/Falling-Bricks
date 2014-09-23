#include "fallingbricks.hpp"

/******************************************************
        CONSTRUCTOR
*******************************************************/
FallingBricks::FallingBricks( )
// initialization list
    : mShouldCount(false),
      mHighScoreObtained(false),
      mSystemRunning(true),
      mPaused(true),
      mTime(0.0f),
      mCounterTime(0.0f),
      mSpeed(0.8f),
      mTickCount(0),
      mScore(0)
{   // body
    srand( time(NULL) ); // seeds the random number generator
    mWindow.create( sf::VideoMode(800, 600, 32), "Falling Bricks", sf::Style::Titlebar | sf::Style::Close ); // creates a window

    mBackgroundMusic.openFromFile("resources/sounds/background.wav");
    mBackgroundMusic.play();
    mBackgroundMusic.setLoop(true);

    mBackgroundMusic2.openFromFile("resources/sounds/background2.wav");

    mpCheerBuffer = std::unique_ptr<sf::SoundBuffer>( new sf::SoundBuffer );
    mpCheerBuffer->loadFromFile("resources/sounds/cheer.wav");
    mpNoHighScoreBuffer = std::unique_ptr<sf::SoundBuffer>( new sf::SoundBuffer );
    mpNoHighScoreBuffer->loadFromFile("resources/sounds/noHighScore.wav");
    mpOnLineClearedSoundBuffer = std::unique_ptr<sf::SoundBuffer>( new sf::SoundBuffer );
    mpOnLineClearedSoundBuffer->loadFromFile("resources/sounds/linecleared.wav");

    mTitleTexture.loadFromFile( "resources/images/title.png" );
    mTitle.setTexture( mTitleTexture );
    mTitle.setPosition( 220, 10 );

    mBGTexture.loadFromFile( "resources/images/background.png" );
    mPlayArea.setTexture( mBGTexture );
    mPlayArea.setPosition( 300, 0 );
    mNextPieceBGTexture.loadFromFile( "resources/images/next_piece_bg.png" );
    mNextPiecePlaceholder.setTexture( mNextPieceBGTexture );
    mNextPiecePlaceholder.setPosition( 60, 10 );

    mControlsTexture.loadFromFile( "resources/images/controls.png" );
    mControls.setTexture( mControlsTexture );
    mControls.setPosition(20, 180 );

    mpBoard = std::shared_ptr<Board>( new Board() );

    sf::Texture purple;
    purple.loadFromFile( "resources/images/purple.png" );
    sf::Texture green;
    green.loadFromFile( "resources/images/green.png" );
    sf::Texture blue;
    blue.loadFromFile( "resources/images/blue.png" );
    sf::Texture orange;
    orange.loadFromFile( "resources/images/orange.png" );
    sf::Texture yellow;
    yellow.loadFromFile( "resources/images/yellow.png" );
    sf::Texture pink;
    pink.loadFromFile( "resources/images/pink.png" );

    mPieceTypes.push_back( std::make_pair(PieceType::l, yellow) );
    mPieceTypes.push_back( std::make_pair(PieceType::L, purple) );
    mPieceTypes.push_back( std::make_pair(PieceType::_l, green) );
    mPieceTypes.push_back( std::make_pair(PieceType::D, blue) );
    mPieceTypes.push_back( std::make_pair(PieceType::T, orange) );
    mPieceTypes.push_back( std::make_pair(PieceType::Z, pink) );

    generateDefaultPieces();

    mFont.loadFromFile("resources/fonts/Anonymous Pro.ttf");
    mScoreboard.setFont( mFont );
    mScoreboard.setCharacterSize(24);
    mScoreboard.setPosition(100, 230);
    mScoreboard.setString("Score: 0");

    mLinesDisplay.setFont( mFont );
    mLinesDisplay.setCharacterSize(24);
    mLinesDisplay.setPosition(100, 250);
    mLinesDisplay.setString( "Lines: 0" );

    mMainMenuStart.setFont( mFont );
    mMainMenuStart.setStyle( sf::Text::Bold );
    mMainMenuStart.setCharacterSize(24);
    mMainMenuStart.setPosition(480, 130);
    mMainMenuStart.setString( "Play" );
    mMainMenuStart.setColor( sf::Color(255, 100, 0) );
    mMainMenuQuit.setFont( mFont );
    mMainMenuQuit.setCharacterSize(24);
    mMainMenuQuit.setPosition(560, 130);
    mMainMenuQuit.setString("Quit");

    mMainMenuHighScores.setFont( mFont );
    mMainMenuHighScores.setStyle( sf::Text::Bold );
    mMainMenuHighScores.setCharacterSize(20);
    mMainMenuHighScores.setPosition(480, 200);
    mMainMenuHighScores.setColor( sf::Color(180, 180, 180) );
    mMainMenuHighScores.setString("High Scores");

    mPostGameMessage.setFont( mFont );
    mPostGameMessage.setCharacterSize(24);
    mPostGameMessage.setPosition( 200, 160 );
    mPostGamePrompt.setFont( mFont );
    mPostGamePrompt.setCharacterSize(24);
    mPostGamePrompt.setPosition( 200, 190 );
    mPostGamePrompt.setString( "Enter your name: " );
    mPostGameName.setFont( mFont );
    mPostGameName.setCharacterSize(24);
    mPostGameName.setPosition( 410, 190 );
    mPostGameName.setString("");
    mPostGameName.setColor( sf::Color(255, 100, 0) );
    mPostGameOk.setFont( mFont );
    mPostGameOk.setPosition( 300, 420 );
    mPostGameOk.setCharacterSize(16);
    mPostGameOk.setString("Press Enter to Continue...");
    mPostGameCursor.setFont( mFont );
    mPostGameCursor.setCharacterSize(24);
    mPostGameCursor.setPosition(410, 190);
    mPostGameCursor.setString("|");

    readScoresFromDisk();
    updateScores();

}
FallingBricks::FallingBricks( const FallingBricks& other ) {}
FallingBricks::~FallingBricks()
{
}

void FallingBricks::generateDefaultPieces()
{   int index;
    index = rand() % 6;
    mpPiece = std::unique_ptr<Piece>( new Piece(mPieceTypes[index].second, mPieceTypes[index].first, mpBoard ) );
    index = rand() % 6;
    mpNextPiece = std::unique_ptr<Piece>( new Piece(mPieceTypes[index].second, mPieceTypes[index].first, mpBoard ) );
    mpNextPiece->moveToPlaceHolder();
}

void FallingBricks::mainloop()
{   pfStateFunc = &FallingBricks::onMainMenu;

    while( mSystemRunning )
    {   mWindow.clear( sf::Color(50, 50, 50) );
        (this->*pfStateFunc)();
        mWindow.display();

    }
}

void FallingBricks::onMainMenu()
{   static bool playSelected = true;
    static float blinkSpeed = 1.5f;
    static float blinkTime = 0.0f;
    static bool playOrange = true;
    static bool quitOrange = false;
    while( mWindow.pollEvent(mEvent) )
    {   switch( mEvent.type )
        {   case sf::Event::Closed:
                mWindow.close();
                mSystemRunning = false;
                break;
            case sf::Event::KeyPressed:
                if( mEvent.key.code == sf::Keyboard::Left || mEvent.key.code == sf::Keyboard::Right )
                {   if( playSelected )
                    {   mMainMenuStart.setStyle(0);
                        mMainMenuStart.setColor( sf::Color::White );
                        mMainMenuQuit.setStyle( sf::Text::Bold );
                        mMainMenuQuit.setColor( sf::Color(255, 100, 0) );
                    }
                    else
                    {   mMainMenuQuit.setStyle( 0 );
                        mMainMenuQuit.setColor( sf::Color::White );
                        mMainMenuStart.setStyle( sf::Text::Bold );
                        mMainMenuStart.setColor( sf::Color(255, 100, 0) );
                    }
                    playSelected = !playSelected;

                }
                if( mEvent.key.code == sf::Keyboard::Return )
                {   if( playSelected )
                    {   mPaused = false;
                        mBackgroundMusic2.play();
                        pfStateFunc = &FallingBricks::onGame;
                    }
                    else
                    {   mSystemRunning = false;
                        break;
                    }
                }
                break;
            default:
                break;
        }
    }
    blinkTime += mClock.getElapsedTime().asSeconds() * blinkSpeed;
    if( blinkTime >= 1.0f )
    {   blinkTime = 0.0f;
        if( playSelected )
        {   if( playOrange )
            {   playOrange = false;
                mMainMenuStart.setColor( sf::Color::White );
            }
            else
            {   playOrange = true;
                mMainMenuStart.setColor( sf::Color(255, 100, 0) );
            }
        }
        else
        {   if( quitOrange )
            {   quitOrange = false;
                mMainMenuQuit.setColor( sf::Color::White );
            }
            else
            {   quitOrange = true;
                mMainMenuQuit.setColor( sf::Color(255, 100, 0) );
            }
        }
    }
    mClock.restart();
    mWindow.draw( mTitle );
    mWindow.draw( mMainMenuStart );
    mWindow.draw( mMainMenuQuit );
    mWindow.draw( mMainMenuHighScores );
    for(unsigned int i = 0; i < mHighScoreDisplays.size(); ++i )
        mWindow.draw( mHighScoreDisplays[i] );
    mWindow.draw( mControls );

}
void FallingBricks::onGame()
{   while( mWindow.pollEvent(mEvent) )
    {   switch( mEvent.type )
        {   case sf::Event::Closed:
                mWindow.close();
                mSystemRunning = false;
                break;
            case sf::Event::KeyPressed:
                if(!mPaused)
                {
                    switch(mEvent.key.code)
                    {
                        case sf::Keyboard::Space:
                            mpPiece->drop();
                            break;
                        case sf::Keyboard::Up:
                            mpPiece->rotate();
                            break;
                        case sf::Keyboard::Left:
                            mpPiece->moveLeft();
                            break;
                        case sf::Keyboard::Right:
                            mpPiece->moveRight();
                            break;
                        case sf::Keyboard::Down:
                            mpPiece->moveDown();
                            break;
                        default: break;
                    }
                }
                break;
            case sf::Event::LostFocus:
                mPaused = true;
                break;
            case sf::Event::GainedFocus:
                mPaused = false;
                break;
            default:
                break;
        }
    }


    mTime += mClock.getElapsedTime().asSeconds() * mSpeed;
    if( mTime >= 1.0f )
    {   mTime = 0.0f;
        mpPiece->onTick( );
        //mSound.setBuffer( *mpOnTickSoundBuffer );
        //mSound.play();
        mTickCount++;
        if( (mTickCount % 20 == 0) && !(mBackgroundMusic2.getStatus() == sf::Music::Status::Playing) )
            mBackgroundMusic2.play();
        if( mpPiece->shouldAssignToBoard() )
        {   mpBoard->give( mpPiece->getBricks() );
            mpPiece = std::move( mpNextPiece );
            mpPiece->moveToStartingPosition();

            int idx = rand() % 6;
            mpNextPiece = std::unique_ptr<Piece>( new Piece(mPieceTypes[idx].second, mPieceTypes[idx].first, mpBoard) );
            mpNextPiece->moveToPlaceHolder();
        }
    }
    mClock.restart();

    unsigned int newScore = mpBoard->getScore();
    if( newScore > mScore ) // if the score has changed
    {   mSpeed += 0.2f; // increase the game speed
        if( mSpeed > 5.f )
            mSpeed = 5.f;
        std::stringstream linestream, scorestream;
        linestream << "Lines: " << mpBoard->getLines();
        mLinesDisplay.setString(linestream.str());
        scorestream << "Score: " << newScore;
        mScoreboard.setString(scorestream.str());
        mScore = newScore;
        mSound.setBuffer( *mpOnLineClearedSoundBuffer );
        mSound.play();
    }
    if( mpBoard->gameOver() )
    {   //std::reverse(mHighScores.begin(), mHighScores.end() );
        if((mHighScores.size() < 10 && mScore > 0) || (mHighScores.size() > 0 && mScore > mHighScores[9].second) )
        {   std::stringstream stream;
            stream << "Congrats, " << mScore << " is a new high score!";
            mPostGameMessage.setString(stream.str());
            mPostGameMessage.setColor( sf::Color::Green );
            mHighScoreObtained = true;

            mSound.setBuffer( *mpCheerBuffer );
            mSound.play();
        }
        else
        {   mPostGameMessage.setString("Sorry, you did not get a new high score.");
            mPostGameMessage.setColor( sf::Color::Red );
            mSound.setBuffer( *mpNoHighScoreBuffer );
            mSound.play();
        }
        mBackgroundMusic2.stop();
        mBackgroundMusic.pause();
        pfStateFunc = &FallingBricks::onPostGame;
    }
    else
    {   // if the game is not over draw all the sprites
        mWindow.draw( mPlayArea );
        mWindow.draw( mNextPiecePlaceholder );
        mWindow.draw( mScoreboard );
        mWindow.draw( mLinesDisplay );
        mpNextPiece->render( &mWindow );
        mpBoard->render( &mWindow );
        mpPiece->render( &mWindow );
    }
}
void FallingBricks::updateScores()
{   mHighScoreDisplays.clear();
    for(unsigned int i = 0; i < mHighScores.size(); ++i )
    {   sf::Text text;
        text.setFont( mFont );
        text.setCharacterSize(16);
        text.setColor( sf::Color(138, 138, 138) );
        text.setPosition(480, 230 + (i * 20) );
        std::stringstream stream;

        stream << mHighScores[i].first << " - ";
        stream << mHighScores[i].second;
        text.setString( stream.str() );

        mHighScoreDisplays.push_back( text );
    }
}

void FallingBricks::onPostGame()
{   static float cursorSpeed = 1.5f;
    static float cursortime = 0.0f;
    while( mWindow.pollEvent(mEvent) )
    {   switch( mEvent.type )
        {   case sf::Event::Closed:
                mWindow.close();
                mSystemRunning = false;
                break;
            case sf::Event::TextEntered:
                if( mHighScoreObtained )
                {   // 13 = return 8 = backspace
                    if (mEvent.text.unicode < 128)
                    {   std::string curName = mPostGameName.getString();
                        if( mEvent.text.unicode == 13 ) // return key
                        {   mHighScores.push_back( std::make_pair(curName, mScore) );
                            std::sort( mHighScores.begin(), mHighScores.end(), FallingBricks::scoreCompare );
                            if( mHighScores.size() > 10 )
                            {   mHighScores.resize( mHighScores.size() - 1 );
                            }
                            saveScoresToDisk();
                            updateScores();
                            resetGame();

                        }
                        else if( mEvent.text.unicode == 8 )
                        {   // backspace
                            if( curName.size() > 0 ) curName.resize(curName.size() - 1);
                            mPostGameName.setString( curName );
                            cursortime = 0.0f;
                            mPostGameCursor.setString("");
                            mPostGameCursor.setPosition( 400 + (14 * curName.size()), 190 );
                        }
                        else if( mEvent.text.unicode != 59 )     // don't allow ";" character (used as a delimeter in file i/o)
                        {   if( curName.size() > 16 ) break;
                            curName += static_cast<char>(mEvent.text.unicode);
                            cursortime = 0.0f;
                            mPostGameCursor.setString("");
                            mPostGameCursor.setPosition( 400 + (14 * curName.size()), 190 );
                            mPostGameName.setString( curName );
                        }
                    }
                }
                else
                {   if( mEvent.text.unicode == 13 ) resetGame();
                }
                break;
            default:
                break;
        }
    }
    mWindow.draw( mPostGameMessage );
    if( mHighScoreObtained )
    {   cursortime += mClock.getElapsedTime().asSeconds() * cursorSpeed;
        if( cursortime >= 1.0f )
        {   cursortime = 0.0f;
            if( mPostGameCursor.getString() == "|" )
                mPostGameCursor.setString("");
            else mPostGameCursor.setString("|");
        }
        mClock.restart();
        mWindow.draw( mPostGamePrompt );
        mWindow.draw( mPostGameName );
        mWindow.draw( mPostGameCursor );
    }
    mWindow.draw( mPostGameOk );
}
void FallingBricks::resetGame()
{   pfStateFunc = &FallingBricks::onMainMenu;
    mPostGameName.setString("");
    mPostGameCursor.setPosition(400, 190);
    mHighScoreObtained = false;
    mpBoard->reset();
    generateDefaultPieces();
    mPaused = true;
    mScore = 0;
    mSpeed = 0.8f;
    mBackgroundMusic.play();
}

std::string FallingBricks::encrypt(std::string input)
{   std::string output = "";
    for(unsigned int i = 0; i < input.size(); ++i )
    {   if( input[i] < 127 && input[i] > 0 ) output += (input[i] + 127);
        else output += input[i];
    }
    return output;
}
std::string FallingBricks::decrypt(std::string input)
{   std::string output = "";
    for(unsigned int i = 0; i < input.size(); ++i)
    {   if( input[i] > 127 || input[i] < 0 ) output += (input[i] - 127);
        else output += input[i];
    }
    return output;
}
void FallingBricks::readScoresFromDisk()
{   mHighScores.clear();
    std::ifstream::pos_type size;
    char* memblock;
    std::ifstream in( "game.data", std::ios::in|std::ios::binary|std::ios::ate );
    if( in.is_open() )
    {   size = in.tellg();
        memblock = new char[size];
        in.seekg(0, std::ios::beg);
        in.read(memblock, size);
        in.close();
        std::string rawData;
        rawData.assign(memblock, size);
        delete[] memblock;

        std::string data = decrypt( rawData );
        std::string name;
        std::string score;
        unsigned int index = 0;
        while( index < data.size() )
        {   score = "";
            name = "";
            while(data[index] != ';' && index < data.size())
            {   name += data[index];
                index++;
            }
            index++;

            while( data[index] != ';' && index < data.size() )
            {   score += data[index];
                index++;
            }
            index++;
            mHighScores.push_back(std::make_pair(name, atoi(score.c_str())));
        }
    }
}
void FallingBricks::saveScoresToDisk()
{   std::ofstream out( "game.data" );
    std::stringstream stream;
    for(unsigned int i = 0; i < mHighScores.size(); ++i)
        stream << mHighScores[i].first << ';' << mHighScores[i].second << ';';
    out << encrypt(stream.str());
    out.close();
}

/*
std::string FallingBricks::encrypt(std::string key, std::string text)
{
    std::string result = "";
    for(int i = 0; i < text.size(); ++i)
    for(int i = 0; i < text.size(); ++i)
    {
        char x = ( (key[i%key.size()] + text[i]) % 127 );
        if( x < 32 )
        {
            printf("That sucks, this letter will probably corrupt the file.\n");
        }
        result += x;
    }

    return result;
}
std::string FallingBricks::decrypt(std::string key, std::string text)
{
    std::string result = "";
    for(int i = 0; i < text.size(); ++i)
    {
        char temp = ( (text[i] - key[i%key.size()]) % 127 );
        if( temp < 0 ) result += (temp + 127);
        else result += temp;
    }
    return result;
}
*/





