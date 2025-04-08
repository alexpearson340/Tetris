#include "tetris/Grid.h"

Grid::Grid(int x, int y, size_t rows, size_t cols)
    : mPosX(x)
    , mPosY(y)
    , mRows(rows)
    , mCols(cols)
    , mGrid(rows, std::vector<Block>(cols))
{
}

void Grid::createBlock(int i, int j, Texture* texture)
{
    int blockX = mPosX + (j * BLOCK_SIZE);
    int blockY = mPosY + (i * BLOCK_SIZE);
    mGrid[i][j] = Block { blockX, blockY, texture };
};

Block& Grid::getBlock(int i, int j)
{
    return mGrid[i][j];
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
    for (int i = 0; i < mRows; ++i)
    {
        for (int j = 0; j < mCols; ++j)
        {
            mGrid[i][j].move(xMove, yMove);
        }
    }

    // Move the Grid ref along with the blocks
    mPosX += xMove;
    mPosY += yMove;
}

void Grid::rotateClockwise()
{
    transpose();
    // Reverse each row
    for (size_t i = 0; i < mRows; ++i)
    {
        std::reverse(mGrid[i].begin(), mGrid[i].end());
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
    for (int i = 0; i < mRows; ++i)
    {
        for (int j = i + 1; j < mRows; ++j)
        {
            std::swap(mGrid[i][j], mGrid[j][i]);
        }
    }
}

void Grid::updatePositions()
{
    for (int i = 0; i < mRows; ++i)
    {
        for (int j = 0; j < mCols; ++j)
        {
            mGrid[i][j].moveTo(mPosX + (j * BLOCK_SIZE), mPosY + (i * BLOCK_SIZE));
        }
    }
}

void Grid::moveRowsDown(int bottomRow, int nRowsToDelete)
{
    for (int r = bottomRow; r >= nRowsToDelete; r--)
    {
        mGrid[r] = mGrid[r - nRowsToDelete];
    }
}

void Grid::render()
{
    for (int i = 0; i < mRows; ++i)
    {
        for (int j = 0; j < mCols; ++j)
        {
            if (mGrid[i][j].exists())
            {
                mGrid[i][j].render();
            };
        }
    }
}

int Grid::getHeight()
{
    return mRows;
}

int Grid::getWidth()
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