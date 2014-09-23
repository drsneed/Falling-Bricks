#ifndef _BOARD_H_
#define _BOARD_H_
#include <vector>
#include <memory>
#include <assert.h>
#include <sfml/graphics.hpp>

typedef std::vector< std::shared_ptr<sf::Sprite> > pSpriteArray;


class Board
{
public:
    static const unsigned int mHeight = 24;
    static const unsigned int mWidth = 16;
    static const unsigned int mYOffset = 14;
    static const unsigned int mXOffset = 310;
    static const unsigned int mBrickSize = 24;
    Board();
    Board( const Board& );
    ~Board();
    bool isValidPosition( float, float );
    void give( pSpriteArray );
    void render( sf::RenderWindow* );
    bool gameOver();
    unsigned int getScore() const;
    unsigned int getLines() const;

    void reset();

private:
    unsigned int mScore;
    unsigned int mLines;
    void handleCompletedLines();
    bool mGameOver;
    std::shared_ptr<sf::Sprite> mBrickPtrs[mWidth][mHeight];
};

#endif
