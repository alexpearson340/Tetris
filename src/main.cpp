#define SDL_MAIN_HANDLED

#include "tetris/Tetris.h"

int main(int argc, char* args[])
{
    TetrisGameEngine tetris {};
    return tetris.run(argc, args);
}
