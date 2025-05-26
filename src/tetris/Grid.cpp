#include "tetris/Grid.h"

Grid::Grid(int x, int y, size_t rows, size_t cols)
    : mPosX(x)
    , mPosY(y)
    , mRows(rows)
    , mCols(cols)
    , mGrid(rows, std::vector<Block>(cols))
{
}

void Grid::createBlock(int xIndex, int yIndex, Texture* texture)
{
    int blockX = mPosX + (xIndex * BLOCK_SIZE);
    int blockY = mPosY + (yIndex * BLOCK_SIZE);
    mGrid[yIndex][xIndex] = Block { blockX, blockY, texture };
};

Block& Grid::getBlock(size_t xIndex, size_t yIndex)
{
    return mGrid[yIndex][xIndex];
}

void Grid::handleEvent(SDL_Event& e)
{
    // If a key was pressed
    if (e.type == SDL_KEYDOWN)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_DOWN:
            setVelY(VERTICAL_FAST_VELOCITY);
            break;
        case SDLK_LEFT:
            setVelX(-1 * BLOCK_SIZE);
            break;
        case SDLK_RIGHT:
            setVelX(BLOCK_SIZE);
            break;
        case SDLK_SPACE:
            mRotate = true;
            break;
        }
    }
    // If a key was released
    else if (e.type == SDL_KEYUP)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_DOWN:
            setVelY(VERTICAL_VELOCITY);
            break;
        case SDLK_LEFT:
            setVelX(0);
            break;
        case SDLK_RIGHT:
            setVelX(0);
            break;
        }
    }
}

void Grid::move(int xMul, int yMul)
{
    bool allowedMove = true;
    int xMove = mVelX * xMul;
    int yMove = mVelY * yMul;

    // Move all the blocks
    forEachBlock(
        [xMove, yMove](Block& block, size_t, size_t)
        {
            block.move(xMove, yMove);
        });

    // Move the Grid ref along with the blocks
    mPosX += xMove;
    mPosY += yMove;
}

void Grid::rotateClockwise()
{
    transpose();
    // Reverse each row
    for (size_t xIndex = 0; xIndex < mCols; ++xIndex)
    {
        std::reverse(mGrid[xIndex].begin(), mGrid[xIndex].end());
    }
    updatePositions();
    mRotate = false;
}

void Grid::rotateAntiClockwise()
{
    rotateClockwise();
    rotateClockwise();
    rotateClockwise();
}

void Grid::transpose()
{
    for (int xIndex = 0; xIndex < mCols; ++xIndex)
    {
        for (int yIndex = xIndex + 1; yIndex < mRows; ++yIndex)
        {
            std::swap(mGrid[xIndex][yIndex], mGrid[yIndex][xIndex]);
        }
    }
}

void Grid::updatePositions()
{
    forEachBlock(
        [this](Block& block, size_t xIndex, size_t yIndex)
        {
            block.moveTo(mPosX + (xIndex * BLOCK_SIZE), mPosY + (yIndex * BLOCK_SIZE));
        });
}

void Grid::moveRowsDown(size_t bottomRow, size_t nRowsToDelete)
{
    // Move rows down
    for (size_t r = bottomRow; r >= nRowsToDelete; r--)
    {
        mGrid[r] = mGrid[r - nRowsToDelete];
    }
    
    // Clear the top rows that are now empty
    for (size_t r = 0; r < nRowsToDelete; r++)
    {
        for (size_t c = 0; c < mCols; c++)
        {
            mGrid[r][c] = Block(); // Create empty block
        }
    }
}

void Grid::render()
{
    forEachBlock(
        [](Block& block, size_t, size_t)
        {
            if (block.exists())
            {
                block.render();
            }
        });
}

size_t Grid::getHeight()
{
    return mRows;
}

size_t Grid::getWidth()
{
    return mCols;
}

int Grid::getPosX()
{
    return mPosX;
}

int Grid::getPosY()
{
    return mPosY;
}

void Grid::setVelX(int velX)
{
    mVelX = velX;
}

void Grid::setVelY(int velY)
{
    mVelY = velY;
}

bool Grid::shouldRotate()
{
    return mRotate;
}