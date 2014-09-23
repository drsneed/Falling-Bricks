#include "piece.hpp"

Piece::Piece( sf::Texture& texture, PieceType type, std::shared_ptr<Board> board )
{
    mpBoard = board;
    setup( texture, type );
}
Piece::Piece() {}
Piece::Piece( const Piece& other ) {}
Piece::~Piece(){ }

bool Piece::shouldAssignToBoard() const
{
    return mAssignToBoard;
}


void Piece::setup( sf::Texture& texture, PieceType type )
{
    speedCheck = 0.0f;
    mAssignToBoard = false;
    mCurrentRot = Rotation::_0;
    mBricks.clear();
    for( int i = 0; i < 4; ++i )
        mBricks.push_back( std::shared_ptr<sf::Sprite>(new sf::Sprite(texture)) );

    int startingPosX = 478;
    mType = type;
    switch( type )
    {
        case PieceType::l:
            mBricks[0]->setPosition(startingPosX, -58);
            mBricks[1]->setPosition(startingPosX, -34);
            mBricks[2]->setPosition(startingPosX, -10);
            mBricks[3]->setPosition(startingPosX, 14);
            break;
        case PieceType::L:
            mBricks[0]->setPosition(startingPosX, -34);
            mBricks[1]->setPosition(startingPosX, -10);
            mBricks[2]->setPosition(startingPosX,  14);
            mBricks[3]->setPosition(startingPosX + mBrickSize, 14 );
            break;
        case PieceType::_l:
            mBricks[0]->setPosition(startingPosX, -34);
            mBricks[1]->setPosition(startingPosX, -10);
            mBricks[2]->setPosition(startingPosX,  14);
            mBricks[3]->setPosition(startingPosX - mBrickSize, 14 );
            break;
        case PieceType::D:
            mBricks[0]->setPosition(startingPosX, -10);
            mBricks[1]->setPosition(startingPosX + mBrickSize, -10);
            mBricks[2]->setPosition(startingPosX, 14 );
            mBricks[3]->setPosition(startingPosX + mBrickSize,  14);
            break;
        case PieceType::T:
            mBricks[0]->setPosition(startingPosX, -34);
            mBricks[1]->setPosition(startingPosX, -10);
            mBricks[2]->setPosition(startingPosX,  14);
            mBricks[3]->setPosition(startingPosX + mBrickSize, -10 );
            break;
        case PieceType::Z:
            mBricks[0]->setPosition(startingPosX, -34);
            mBricks[1]->setPosition(startingPosX, -10);
            mBricks[2]->setPosition(startingPosX + mBrickSize, -10 );
            mBricks[3]->setPosition(startingPosX + mBrickSize,  14);
            break;
    }

}

void Piece::moveToPlaceHolder()
{
    int x = 0, y = 0;
    switch( mType )
    {
        case PieceType::l: x = -355; y = 93; break;
        case PieceType::L: x = -360; y = 85; break;
        case PieceType::_l: x = -347; y = 85; break;
        case PieceType::D: x = -363; y = 72; break;
        case PieceType::T: x = -360; y = 86; break;
        case PieceType::Z: x = -357; y = 87; break;
    }
    for(int i = 0; i < mBricks.size(); ++i )
    {
        mBricks[i]->move(x, y);
    }
}
void Piece::moveToStartingPosition()
{
    int x = 0, y = 0;
    switch( mType )
    {
        case PieceType::l: x = 355; y = -93; break;
        case PieceType::L: x = 360; y = -85; break;
        case PieceType::_l: x = 347; y = -85; break;
        case PieceType::D: x = 363; y = -72; break;
        case PieceType::T: x = 360; y = -86; break;
        case PieceType::Z: x = 357; y = -87; break;
    }
    for(int i = 0; i < mBricks.size(); ++i )
    {
        mBricks[i]->move(x, y);
    }
}

pSpriteArray Piece::getBricks() const
{
    return mBricks;
}

void Piece::rotate()
{
    switch( mCurrentRot )
    {
    case Rotation::_0:
        rotate_90();
        break;
    case Rotation::_90:
        rotate_180();
        break;
    case Rotation::_180:
        rotate_270();
        break;
    case Rotation::_270:
        rotate_0();
        break;
    }

}

void Piece::rotate_90()
{
    float oldX0 = mBricks[0]->getPosition().x;
    float oldX1 = mBricks[1]->getPosition().x;
    float oldX2 = mBricks[2]->getPosition().x;
    float oldX3 = mBricks[3]->getPosition().x;
    float oldY0 = mBricks[0]->getPosition().y;
    float oldY1 = mBricks[1]->getPosition().y;
    float oldY2 = mBricks[2]->getPosition().y;
    float oldY3 = mBricks[3]->getPosition().y;

    std::vector<sf::Vector2f> newPos(4);
    switch( mType )
    {
    case PieceType::l:
        newPos[0].x = oldX0 + (mBrickSize * 2);
        newPos[0].y = oldY0 + (mBrickSize * 3);
        newPos[1].x = oldX1 +  mBrickSize;
        newPos[1].y = oldY1 + (mBrickSize * 2);
        newPos[2].x = oldX2;
        newPos[2].y = oldY2 +  mBrickSize;
        newPos[3].x = oldX3 -  mBrickSize;
        newPos[3].y = oldY3;
        break;

    case PieceType::L:
        newPos[0].x = oldX0 +  mBrickSize;
        newPos[0].y = oldY0 +  mBrickSize;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2 -  mBrickSize;
        newPos[2].y = oldY2 -  mBrickSize;
        newPos[3].x = oldX3 - (mBrickSize * 2);
        newPos[3].y = oldY3;
        break;

    case PieceType::_l:
        newPos[0].x = oldX0 + (mBrickSize * 2);
        newPos[0].y = oldY0 + (mBrickSize * 2);
        newPos[1].x = oldX1 +  mBrickSize;
        newPos[1].y = oldY1 +  mBrickSize;
        newPos[2].x = oldX2;
        newPos[2].y = oldY2;
        newPos[3].x = oldX3 +  mBrickSize;
        newPos[3].y = oldY3 -  mBrickSize;
        break;

    case PieceType::T:
        newPos[0].x = oldX0 +  mBrickSize;
        newPos[0].y = oldY0 +  mBrickSize;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2 - mBrickSize;
        newPos[2].y = oldY2 - mBrickSize;
        newPos[3].x = oldX3 - mBrickSize;
        newPos[3].y = oldY3 + mBrickSize;
        break;

    case PieceType::Z:
        newPos[0].x = oldX0 + mBrickSize;
        newPos[0].y = oldY0 + mBrickSize;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2 - mBrickSize;
        newPos[2].y = oldY2 + mBrickSize;
        newPos[3].x = oldX3 - (mBrickSize * 2);
        newPos[3].y = oldY3;
        break;

    default:
        return;
    }
    bool success = checkAndRotate( newPos );
    if( success ) mCurrentRot = Rotation::_90;
}
void Piece::rotate_180()
{
    float oldX0 = mBricks[0]->getPosition().x;
    float oldX1 = mBricks[1]->getPosition().x;
    float oldX2 = mBricks[2]->getPosition().x;
    float oldX3 = mBricks[3]->getPosition().x;
    float oldY0 = mBricks[0]->getPosition().y;
    float oldY1 = mBricks[1]->getPosition().y;
    float oldY2 = mBricks[2]->getPosition().y;
    float oldY3 = mBricks[3]->getPosition().y;

    std::vector<sf::Vector2f> newPos(4);
    switch( mType )
    {
    case PieceType::l:
        newPos[0].x = oldX0 - (mBrickSize * 2);
        newPos[0].y = oldY0;
        newPos[1].x = oldX1 -  mBrickSize;
        newPos[1].y = oldY1 -  mBrickSize;
        newPos[2].x = oldX2;
        newPos[2].y = oldY2 - (mBrickSize * 2);
        newPos[3].x = oldX3 +  mBrickSize;
        newPos[3].y = oldY3 - (mBrickSize * 3);
        break;
    case PieceType::L:
        newPos[0].x = oldX0 - mBrickSize;
        newPos[0].y = oldY0 + mBrickSize;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2 + mBrickSize;
        newPos[2].y = oldY2 - mBrickSize;
        newPos[3].x = oldX3;
        newPos[3].y = oldY3 - (mBrickSize * 2);
        break;

    case PieceType::_l:
        newPos[0].x = oldX0 - (mBrickSize * 2);
        newPos[0].y = oldY0;
        newPos[1].x = oldX1 -  mBrickSize;
        newPos[1].y = oldY1 -  mBrickSize;
        newPos[2].x = oldX2;
        newPos[2].y = oldY2 - (mBrickSize * 2);
        newPos[3].x = oldX3 +  mBrickSize;
        newPos[3].y = oldY3 -  mBrickSize;
        break;

    case PieceType::T:
        newPos[0].x = oldX0 -  mBrickSize;
        newPos[0].y = oldY0 +  mBrickSize;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2 + mBrickSize;
        newPos[2].y = oldY2 - mBrickSize;
        newPos[3].x = oldX3 - mBrickSize;
        newPos[3].y = oldY3 - mBrickSize;
        break;

    case PieceType::Z:
        newPos[0].x = oldX0 - mBrickSize;
        newPos[0].y = oldY0 + mBrickSize;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2 - mBrickSize;
        newPos[2].y = oldY2 - mBrickSize;
        newPos[3].x = oldX3;
        newPos[3].y = oldY3 - (mBrickSize * 2);
        break;

    default:
        return;
    }
    bool success = checkAndRotate( newPos );
    if( success ) mCurrentRot = Rotation::_180;
}
void Piece::rotate_270()
{
    float oldX0 = mBricks[0]->getPosition().x;
    float oldX1 = mBricks[1]->getPosition().x;
    float oldX2 = mBricks[2]->getPosition().x;
    float oldX3 = mBricks[3]->getPosition().x;
    float oldY0 = mBricks[0]->getPosition().y;
    float oldY1 = mBricks[1]->getPosition().y;
    float oldY2 = mBricks[2]->getPosition().y;
    float oldY3 = mBricks[3]->getPosition().y;

    std::vector<sf::Vector2f> newPos(4);
    switch( mType )
    {
    case PieceType::l:
        newPos[0].x = oldX0 -  mBrickSize;
        newPos[0].y = oldY0;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 +  mBrickSize;
        newPos[2].x = oldX2 +  mBrickSize;
        newPos[2].y = oldY2 + (mBrickSize * 2);
        newPos[3].x = oldX3 + (mBrickSize * 2);
        newPos[3].y = oldY3 + (mBrickSize * 3);
        break;
    case PieceType::L:
        newPos[0].x = oldX0 -  mBrickSize;
        newPos[0].y = oldY0;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 +  mBrickSize;
        newPos[2].x = oldX2 +  mBrickSize;
        newPos[2].y = oldY2 + (mBrickSize * 2);
        newPos[3].x = oldX3 + (mBrickSize * 2);
        newPos[3].y = oldY3 +  mBrickSize;
        break;

    case PieceType::_l:
        newPos[0].x = oldX0 - (mBrickSize * 2);
        newPos[0].y = oldY0 -  mBrickSize;
        newPos[1].x = oldX1 -  mBrickSize;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2;
        newPos[2].y = oldY2 +  mBrickSize;
        newPos[3].x = oldX3 -  mBrickSize;
        newPos[3].y = oldY3 + (mBrickSize * 2);
        break;

    case PieceType::T:
        newPos[0].x = oldX0 - mBrickSize;
        newPos[0].y = oldY0;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 + mBrickSize;
        newPos[2].x = oldX2 + mBrickSize;
        newPos[2].y = oldY2 + (mBrickSize * 2);
        newPos[3].x = oldX3 + mBrickSize;
        newPos[3].y = oldY3;
        break;

    case PieceType::Z:
        newPos[0].x = oldX0 - mBrickSize;
        newPos[0].y = oldY0;
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 + mBrickSize;
        newPos[2].x = oldX2 + mBrickSize;
        newPos[2].y = oldY2;
        newPos[3].x = oldX3 + (mBrickSize * 2);
        newPos[3].y = oldY3 + mBrickSize;
        break;

    default:
        return;
    }
    bool success = checkAndRotate( newPos );
    if( success ) mCurrentRot = Rotation::_270;
}

void Piece::rotate_0()
{
    float oldX0 = mBricks[0]->getPosition().x;
    float oldX1 = mBricks[1]->getPosition().x;
    float oldX2 = mBricks[2]->getPosition().x;
    float oldX3 = mBricks[3]->getPosition().x;
    float oldY0 = mBricks[0]->getPosition().y;
    float oldY1 = mBricks[1]->getPosition().y;
    float oldY2 = mBricks[2]->getPosition().y;
    float oldY3 = mBricks[3]->getPosition().y;

    std::vector<sf::Vector2f> newPos(4);
    switch( mType )
    {
    case PieceType::l:
        newPos[0].x = oldX0 +  mBrickSize;
        newPos[0].y = oldY0 - (mBrickSize * 3);
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 - (mBrickSize * 2);
        newPos[2].x = oldX2 -  mBrickSize;
        newPos[2].y = oldY2 -  mBrickSize;
        newPos[3].x = oldX3 - (mBrickSize * 2);
        newPos[3].y = oldY3;
        break;
    case PieceType::L:
        newPos[0].x = oldX0 +  mBrickSize;
        newPos[0].y = oldY0 - (mBrickSize * 2);
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 -  mBrickSize;
        newPos[2].x = oldX2 -  mBrickSize;
        newPos[2].y = oldY2;
        newPos[3].x = oldX3;
        newPos[3].y = oldY3 +  mBrickSize;
        break;

    case PieceType::_l:
        newPos[0].x = oldX0 + (mBrickSize * 2);
        newPos[0].y = oldY0 -  mBrickSize;
        newPos[1].x = oldX1 +  mBrickSize;
        newPos[1].y = oldY1;
        newPos[2].x = oldX2;
        newPos[2].y = oldY2 +  mBrickSize;
        newPos[3].x = oldX3 -  mBrickSize;
        newPos[3].y = oldY3;
        break;

    case PieceType::T:
        newPos[0].x = oldX0 + mBrickSize;
        newPos[0].y = oldY0 - (mBrickSize * 2);
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 - mBrickSize;
        newPos[2].x = oldX2 - mBrickSize;
        newPos[2].y = oldY2;
        newPos[3].x = oldX3 + mBrickSize;
        newPos[3].y = oldY3;
        break;

    case PieceType::Z:
        newPos[0].x = oldX0 + mBrickSize;
        newPos[0].y = oldY0 - (mBrickSize * 2);
        newPos[1].x = oldX1;
        newPos[1].y = oldY1 - mBrickSize;
        newPos[2].x = oldX2 + mBrickSize;
        newPos[2].y = oldY2;
        newPos[3].x = oldX3;
        newPos[3].y = oldY3 + mBrickSize;
        break;

    default:
        return;
    }
    bool success = checkAndRotate( newPos );
    if( success ) mCurrentRot = Rotation::_0;
}

bool Piece::checkAndRotate( std::vector<sf::Vector2f>& newPos )
{
    bool good = true;
    for(int i = 0; i < 4; ++i)
        if(! mpBoard->isValidPosition(newPos[i].x, newPos[i].y) )
        {   good = false;
            break;
        }
    if( good )
        for(int i = 0; i < mBricks.size(); ++i )
            mBricks[i]->setPosition( newPos[i] );
    return good;
}

void Piece::moveDown()
{
    if( canMoveDown() )
    {
        for(int i = 0; i < mBricks.size(); ++i)
        {
            mBricks[i]->move(0, mBrickSize);
        }
    }
}
void Piece::moveLeft()
{
    if( canMoveLeft() )
    {
        for(int i = 0; i < mBricks.size(); ++i)
        {
            mBricks[i]->move(-24, 0);
        }
    }
}
void Piece::moveRight()
{
    if( canMoveRight() )
    {
        for(int i = 0; i < mBricks.size(); ++i)
        {
            mBricks[i]->move(mBrickSize, 0);
        }
    }
}

bool Piece::canMoveLeft()
{
    for( int i = 0; i < mBricks.size(); ++i )
    {
        sf::Vector2f pos = mBricks[i]->getPosition();
        if(! mpBoard->isValidPosition(pos.x - mBrickSize, pos.y) )
        {
            return false;
        }

    }
    return true;
}
void Piece::drop()
{
    bool shouldContinue = true;
    while( shouldContinue )
    {   for(int i = 0; i < mBricks.size(); ++i )
        {   sf::Vector2f pos = mBricks[i]->getPosition();
            if(! mpBoard->isValidPosition(pos.x, pos.y + mBrickSize) )
                shouldContinue = false;
        }
        if( shouldContinue )
        {   for(int i = 0; i < mBricks.size(); ++i )
            {
                mBricks[i]->move(0, mBrickSize);
            }
        }
    }

}
bool Piece::canMoveRight()
{
    for( int i = 0; i < mBricks.size(); ++i )
    {
        sf::Vector2f pos = mBricks[i]->getPosition();
        if(! mpBoard->isValidPosition(pos.x + mBrickSize, pos.y) )
        {
            return false;
        }

    }
    return true;
}

bool Piece::canMoveDown()
{
    for( int i = 0; i < mBricks.size(); ++i )
    {
        sf::Vector2f pos = mBricks[i]->getPosition();
        if(! mpBoard->isValidPosition(pos.x, pos.y + mBrickSize) )
        {
            return false;
        }

    }
    return true;
}

void Piece::onTick( )
{
    if( canMoveDown() )
    {
        for( int i = 0; i < mBricks.size(); ++i )
            mBricks[i]->move( 0, mBrickSize );
    } else {
        mAssignToBoard = true;
    }
}

void Piece::render( sf::RenderWindow* window ) const
{
    for(int i = 0; i < mBricks.size(); ++i )
    {   if( mBricks[i]->getPosition().y >= 14 )
            window->draw( *(mBricks[i]) );
    }
}
