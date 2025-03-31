#ifndef GRID_H
#define GRID_H

#include "Block.h"
#include "Constants.h"
#include <sstream>

// A class wrapping a 2D grid of Blocks.
// The game board, as well as individual Tetronimos are
// stored in a Grid. Provides rotating functionality
class Grid
{
public:
    Grid(int, int, size_t, size_t);

    // Takes key presses and adjusts the Block's velocity
    void handleEvent(SDL_Event& e);

    void move(int, int);

    void createBlock(int, int, Texture*);

    Block& getBlock(int, int);
    void rotateClockwise();
    void rotateAntiClockwise();
    void render();

    int getHeight();

    int getWidth();

    int getPosX();

    int getPosY();

    bool shouldRotate();

    void updatePositions();

    void moveRowsDown(int, int);

private:
    // The X and Y offsets of the grid
    int mPosX, mPosY;

    // The velocity of the grid
    int mVelX = 0;
    int mVelY = VERTICAL_VELOCITY;
    bool mRotate = false; // The user has pressed the rotate key and it should
                          // rotate this frame

    void transpose();

    size_t mRows;
    size_t mCols;
    std::vector<std::vector<Block>> mGrid; // 2D vector of Blocks or nulls
};

#endif