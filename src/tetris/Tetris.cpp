#include "tetris/Tetris.h"
#include <iostream>

TetrisGameEngine::TetrisGameEngine()
    : BaseEngine(SCREEN_HEIGHT, SCREEN_WIDTH)
    , mCurrentTetronimo { 0, 0, 0, 0 }
    , mGameBoard { 0, 0, 0, 0 }
    , mFactory { mTextures }
    , mCollisionHandler { nullptr, nullptr, 0 }
    , mInfoBar {}
    , mInfoText {} {
    };

bool TetrisGameEngine::loadMedia()
{
    bool success = true;
    mTextures.clear();

    // Open the block textures
    success = success && loadTexture(BLOCK_TEXTURE_RED);
    success = success && loadTexture(BLOCK_TEXTURE_BLUE);
    success = success && loadTexture(BLOCK_TEXTURE_YELLOW);
    success = success && loadTexture(BLOCK_TEXTURE_GREEN);
    success = success && loadTexture(BLOCK_TEXTURE_PURPLE);
    success = success && loadTexture(BLOCK_TEXTURE_ORANGE);
    success = success && loadTexture(BLOCK_TEXTURE_NAVY);
    success = success && loadTexture(BLOCK_TEXTURE_GREY);
    success = success && loadTexture(BLOCK_TEXTURE_WHITE);
    success = success && loadTexture(BLOCK_TEXTURE_BLACK);
    return success;
}

bool TetrisGameEngine::create()
{
    mGameBoard = Grid(0, 0, N_ROWS, N_COLS);
    mGameBoard.updatePositions();
    mCollisionHandler = CollisionHandler(
        mTextures.at(BLOCK_TEXTURE_WHITE).get(),
        mTextures.at(BLOCK_TEXTURE_BLACK).get(),
        mElapsedTime);
    mCurrentTetronimo = mFactory.getNextTetronimo();
    
    // Initialize the information bar text(ure)
    mInfoBar = std::make_unique<Texture>(mRenderer.get(), mFont.get());
    updateInformationBar();
    
    return true;
}

void TetrisGameEngine::updateInformationBar()
{
    mInfoText.str("");
    mInfoText << "  fps  " << mFps << "  |  score  " << mScore;
    if (!mInfoBar->loadFromRenderedText(
            mInfoText.str().c_str(), TEXT_COLOUR, BACKGROUND_COLOUR))
    {
        printf("Failed to load text texture\n");
    }
}

bool TetrisGameEngine::update()
{
    updateInformationBar();
    
    // Handle events on queue
    while (SDL_PollEvent(&mEvent) != 0)
    {
        // User requests quit
        if (mEvent.type == SDL_QUIT)
        {
            mQuit = true;
        }

        if (mPlaying)
        {
            // Handle input for the block
            mCurrentTetronimo.handleEvent(mEvent);
        }
    }

    if (mPlaying)
    {
        // Handle movement and collisions
        if (mCollisionHandler.handle(mCurrentTetronimo, mGameBoard, SDL_GetTicks()))
        {
            mCurrentTetronimo = mFactory.getNextTetronimo();
            mScore = mScore + 4;
            updateInformationBar();
        };
        mPlaying = mCollisionHandler.keepPlaying();
    }
    return true;
}

bool TetrisGameEngine::render()
{
    // Draw the start line
    SDL_SetRenderDrawColor(mRenderer.get(), 0xC8, 0xC8, 0xC8, 0xFF);
    SDL_RenderDrawLine(mRenderer.get(), 0, START_LINE, mScreenWidth, START_LINE);

    // Render game state objects
    mGameBoard.render();
    mCurrentTetronimo.render();
    mInfoBar->render(0, mScreenHeight - BOTTOM_BAR_HEIGHT);
    return true;
}