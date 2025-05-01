#ifndef GRID_H
#define GRID_H

#include "tetris/Block.h"
#include "tetris/Constants.h"
#include <sstream>
#include <vector>

// A class wrapping a 2D grid of Blocks.
// The game board, as well as individual Tetronimos are
// stored in a Grid. Provides rotating functionality
class Grid
{
public:
    Grid(int, int, size_t, size_t);

    // Takes key presses and adjusts the Block's velocity
    void handleEvent(SDL_Event& e);

    // Call a function on each block in the Grid
    template <typename Func>
    void forEachBlock(Func&& func)
    {
        for (int xIndex = 0; xIndex < mCols; ++xIndex)
        {
            for (int yIndex = 0; yIndex < mRows; ++yIndex)
            {
                func(mGrid[yIndex][xIndex], xIndex, yIndex);
            }
        }
    }

    void move(int, int);

    void createBlock(int, int, Texture*);

    Block& getBlock(size_t, size_t);
    void rotateClockwise();
    void rotateAntiClockwise();
    void render();

    size_t getHeight();

    size_t getWidth();

    int getPosX();

    int getPosY();

    void setVelX(int);

    void setVelY(int);

    bool shouldRotate();

    void updatePositions();

    void moveRowsDown(size_t, size_t);

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