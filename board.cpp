#include "board.hpp"

Board::Board()
    : mGameOver( false ),
      mScore( 0 ),
      mLines( 0 )
{
    reset();
}

Board::Board( const Board& other ) {}
Board::~Board()
{
    printf("destroying board\n");
}

bool Board::isValidPosition( float x, float y )
{
    if( x > mXOffset + 384 || y + mBrickSize > mYOffset + 576 || x < mXOffset )
    {
        return false;
    }
    if( y < mYOffset ) return true;
    unsigned int i = (static_cast<unsigned int>(x) - mXOffset) / mBrickSize;
    unsigned int j = (static_cast<unsigned int>(y) - mYOffset) / mBrickSize;
    if(! (i >= 0 && i <= 15 && j >= 0 && j <= 23) )
        return false;
    if( mBrickPtrs[i][j] )
    {
        return false;
    }
    return true;
}

unsigned int Board::getScore() const { return mScore; }
unsigned int Board::getLines() const { return mLines; }

void Board::reset()
{
    mGameOver = false;
    mScore = 0;
    mLines = 0;
    for(unsigned int i = 0; i < mWidth; ++i)
        for(unsigned int j = 0; j < mHeight; ++j)
            mBrickPtrs[i][j] = nullptr;
}

void Board::give( pSpriteArray bricks )
{
    unsigned int x, y;
    for( int i = 0; i < bricks.size(); ++i )
    {
       sf::Vector2f pos = bricks[i]->getPosition();
       if( pos.y < 0 )
       {
           mGameOver = true;
           return;
       }
       x = (static_cast<unsigned int>(pos.x) - 310) / 24;
       y = (static_cast<unsigned int>(pos.y) - 14 ) / 24;
       mBrickPtrs[x][y] = bricks[i];
    }
    handleCompletedLines();
}

void Board::handleCompletedLines()
{
    /*\
    |*| Function which takes care of removing lines that have been filled up,
    |*| then awards points to the user.
    \*/
    std::vector<unsigned int> linesToRemove; // keeps the indices of completed lines
    for(unsigned int j = mHeight - 1; j != 0; --j ) // cycle from bottom to top
    {
        bool lineComplete = true; // is this line filled up?
        for(unsigned int i = 0; i < mWidth; ++i )
        {
            if( mBrickPtrs[i][j] == nullptr ) lineComplete = false; // no it's not
        }
        if( lineComplete ) // yes it is
        {
            linesToRemove.push_back( j ); // add the index to the vector
        }
    }
    // how many lines do we need to remove?
    unsigned int totalLines = linesToRemove.size();
    // each line that is removed moves all blocks above it downwards
    // therefore, the indices in the vector will incrementally be off.
    // by one each cycle. The offsetter variable "offsets" this increment.
    unsigned int offsetter = 0;
    for( int count = 0; count < totalLines; ++count ) // for each line to remove
    {
        unsigned int j = linesToRemove[count] + offsetter; // add the offsetter to the index
        if( j == 0 ) // are we at the very top?
        {   for( unsigned int i = 0; i < mWidth; ++i )
                mBrickPtrs[i][j] = nullptr; // set each block in the row to null
        } else {    // otherwise
            for( unsigned int idx = j; idx > 0; --idx ) // for each block in this row and all above it
            {
               for( unsigned int i = 0; i < mWidth; ++i )
                {
                    mBrickPtrs[i][idx] = mBrickPtrs[i][idx - 1]; // this block equals the one above it
                    if( mBrickPtrs[i][idx] ) mBrickPtrs[i][idx]->move(0, 24); // if not empty, move sprite down
                    mBrickPtrs[i][idx - 1] = nullptr; // set old block to null
                }
            }
        }
        offsetter++; // increment the offsetter each cycle through the loop
    }

    mLines += totalLines; // keep track of how many lines the player has cleared
    // give the player some points for the effort. the more rows at once, the more points.
    switch( totalLines )
    {
    case 4:
        mScore += 400;
        break;
    case 3:
        mScore += 200;
        break;
    case 2:
        mScore += 50;
        break;
    case 1:
        mScore += 10;
        break;
    }
}

bool Board::gameOver()
{
    return mGameOver;
}
void Board::render( sf::RenderWindow* window )
{
    for(unsigned int i = 0; i < mWidth; ++i )
        for(unsigned int j = 0; j < mHeight; ++j )
            if( mBrickPtrs[i][j] )
                window->draw( *(mBrickPtrs[i][j]) );
}
