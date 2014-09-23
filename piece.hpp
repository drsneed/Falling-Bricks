#ifndef _PIECE_H_
#define _PIECE_H_
#include <sfml/graphics.hpp>
#include <memory>
#include <vector>
#include <assert.h>
#include "board.hpp"
enum class Direction : int8_t
{
    Left,
    Right,
    Down
};

enum class PieceType : int8_t
{
    l,
    L,
    _l,
    D,
    T,
    Z
};

enum class Rotation : int8_t
{
    _0,
    _90,
    _180,
    _270
};

//typedef std::vector< std::shared_ptr<sf::Sprite> > pSpriteArray;



class Piece
{
public:
    static void customDelete(sf::Sprite* ptr);
    static const unsigned int mYOffset = 14;
    static const unsigned int mXOffset = 310;
    static const unsigned int mBrickSize = 24;
    Piece( sf::Texture&, PieceType, std::shared_ptr<Board> );
    Piece();
    Piece( const Piece& );
    ~Piece();

    void setup( sf::Texture&, PieceType, pSpriteArray* );
    void setup( sf::Texture&, PieceType );
    void moveDown();
    void moveLeft();
    void moveRight();
    void drop();
    void rotate();
    void onTick( );
    pSpriteArray getBricks() const;
    void printType();
    void render( sf::RenderWindow* ) const;
    bool shouldAssignToBoard() const;
    void moveToPlaceHolder();
    void moveToStartingPosition();
private:
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    void rotate_0();
    void rotate_90();
    void rotate_180();
    void rotate_270();
    bool checkAndRotate( std::vector<sf::Vector2f>& );
private:
    Rotation mCurrentRot;
    std::shared_ptr<Board> mpBoard;
    float speedCheck;
    PieceType mType;
    bool mAssignToBoard;
    pSpriteArray mBricks;
};


#endif
