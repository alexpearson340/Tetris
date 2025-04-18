#include "tetris/CollisionHandler.h"

CollisionHandler::CollisionHandler(Texture* whiteFlashTexture, Texture* blackFlashTexture, uint32_t currentTime)
    : mPreviousTime { 0 }
    , mCurrentTime { currentTime }
    , mWhiteFlashTexture { whiteFlashTexture }
    , mBlackFlashTexture { blackFlashTexture }
{
}

bool CollisionHandler::keepPlaying()
{
    return mKeepPlaying;
}

bool CollisionHandler::handle(Grid& tetronimo, Grid& gameBoard, uint32_t currentTime)
{
    mCurrentTime = currentTime;

    // If we are midway through animating a completed row, do this branch instead
    if (mFinishedRowRoutine)
    {
        return animateCompletedRows(gameBoard);
    }

    // Cap horizontal movements and rotations to a certain frequency
    if ((mCurrentTime - mPreviousTime) >= INPUT_INTERVAL_MS)
    {
        mPreviousTime = mCurrentTime;
        handleHorizontal(tetronimo, gameBoard);
        handleRotational(tetronimo, gameBoard);
    }

    // Move the block vertically - return whether we need a new tetronimo
    return handleVertical(tetronimo, gameBoard);
}

void CollisionHandler::handleHorizontal(Grid& tetronimo, Grid& gameBoard)
{
    // Move the block horizontally
    tetronimo.move(1, 0);
    if (!checkCollisions(tetronimo, gameBoard))
    {
        tetronimo.move(-1, 0);
    };
}

void CollisionHandler::handleRotational(Grid& tetronimo, Grid& gameBoard)
{
    // Rotate the block
    if (tetronimo.shouldRotate())
    {
        tetronimo.rotateClockwise();
        if (!checkCollisions(tetronimo, gameBoard))
        {
            tetronimo.rotateAntiClockwise();
        }
    };
}

bool CollisionHandler::handleVertical(Grid& tetronimo, Grid& gameBoard)
{
    bool newTetronimoRequired = false;
    tetronimo.move(0, 1);
    if (!checkCollisions(tetronimo, gameBoard))
    {
        // freeze routine
        freezeTetronimo(tetronimo, gameBoard);

        // handle if the player has completed any rows
        handleCompletedRows(tetronimo, gameBoard);

        newTetronimoRequired = true;
    };
    return newTetronimoRequired;
}

void CollisionHandler::freezeTetronimo(Grid& tetronimo, Grid& gameBoard)
{
    // find nearest whole number of blocks on game board
    int rowOnGameBoard = tetronimo.getPosY() / BLOCK_SIZE;
    int colOnGameBoard = tetronimo.getPosX() / BLOCK_SIZE;

    for (int i = 0; i < tetronimo.getHeight(); ++i)
    {
        for (int j = 0; j < tetronimo.getWidth(); ++j)
        {
            Block& block = tetronimo.getBlock(i, j);
            if (block.exists())
            {
                gameBoard.createBlock(
                    rowOnGameBoard + i, colOnGameBoard + j, block.getTexture());
                if ((rowOnGameBoard + i) * BLOCK_SIZE < START_LINE)
                {
                    mKeepPlaying = false;
                }
            }
        }
    }
}

void CollisionHandler::handleCompletedRows(Grid& tetronimo, Grid& gameBoard)
{
    // work out which rows have been completed
    int rowOnGameBoard = tetronimo.getPosY() / BLOCK_SIZE;
    int colOnGameBoard = tetronimo.getPosX() / BLOCK_SIZE;
    int rowNum;
    std::vector<int> completedRows;
    for (int i = 0; i < tetronimo.getHeight(); ++i)
    {
        int rowNum = rowOnGameBoard + i;
        if ((rowNum < gameBoard.getHeight()) && checkForCompletedRow(rowNum, gameBoard))
        {
            mCompletedRows.push_back(rowNum);
        }
    }

    if (mCompletedRows.size() > 0)
    {
        // start the finished row animation routine
        mFinishedRowRoutine = true;
        setFlashingTexture(mCompletedRows, gameBoard, mBlackFlashTexture);
    }
}

bool CollisionHandler::checkForCompletedRow(int rowNum, Grid& gameBoard)
{
    for (int j = 0; j < gameBoard.getWidth(); ++j)
    {
        if (!gameBoard.getBlock(rowNum, j).exists())
        {
            return false;
        }
    }
    return true;
}

bool CollisionHandler::animateCompletedRows(Grid& gameBoard)
{
    if (mCurrentTime >= mFlashRowTransitionTime)
    {
        setFlashingTexture(mCompletedRows, gameBoard, (mNumberOfFlashesRemaining % 2 == 0) ? mBlackFlashTexture : mWhiteFlashTexture);
    }

    // finish the completed row routine - delete the completed rows and move existing rows down
    if (mNumberOfFlashesRemaining == 0)
    {
        gameBoard.moveRowsDown(mCompletedRows.back(), mCompletedRows.size());
        gameBoard.updatePositions();
        mCompletedRows.clear();
        mFinishedRowRoutine = false;
        mNumberOfFlashesRemaining = N_ROW_FLASHES;
    }
    return false;
}

// Visual effect when the player completes a row
void CollisionHandler::setFlashingTexture(std::vector<int> completedRows,
    Grid& gameBoard,
    Texture* texture)
{
    for (int rowNum : completedRows)
    {
        for (int j = 0; j < gameBoard.getWidth(); ++j)
        {
            gameBoard.getBlock(rowNum, j).setTexture(texture);
        }
    }
    mNumberOfFlashesRemaining -= 1;
    mFlashRowTransitionTime = mCurrentTime + COMPLETED_ROW_FLASH_INTERVAL_MS;
}

bool CollisionHandler::checkCollisions(Grid& tetronimo, Grid& gameBoard)
{
    // find nearest whole number of blocks on game board
    int rowOnGameBoard = tetronimo.getPosY() / BLOCK_SIZE;
    int colOnGameBoard = tetronimo.getPosX() / BLOCK_SIZE;

    // bounday condition for the very bottom row
    rowOnGameBoard += (rowOnGameBoard >= (gameBoard.getHeight() - tetronimo.getHeight())) ? 0 : 1;

    for (int i = 0; i < tetronimo.getWidth(); ++i)
    {
        for (int j = 0; j < tetronimo.getHeight(); ++j)
        {
            Block& block = tetronimo.getBlock(i, j);
            if (block.exists())
            {
                // Check sides of screen
                if ((block.getPosX() < 0) || (block.getPosX() + BLOCK_SIZE > SCREEN_WIDTH))
                {
                    return false;
                }
                // Check top and bottom of playable area
                if ((block.getPosY() < 0) || (block.getPosY() + BLOCK_SIZE > (N_ROWS * BLOCK_SIZE)))
                {
                    return false;
                }
                // Check for an already frozen Tetronimo in the same position as the
                // Block
                if (gameBoard.getBlock(rowOnGameBoard + i, colOnGameBoard + j)
                        .exists())
                {
                    return false;
                }
            }
        }
    }
    return true;
}