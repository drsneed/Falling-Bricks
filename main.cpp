#include "fallingbricks.hpp"

int main( int argc, char* argv[] )
{
    FallingBricks* fallingBricksGame = new FallingBricks();
    fallingBricksGame->mainloop();
    delete fallingBricksGame;
    return 0;
}
