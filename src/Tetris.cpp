#include "tetris/Tetris.h"
#include <iostream>

TetrisGameEngine::TetrisGameEngine()
    : GameEngine(SCREEN_HEIGHT, SCREEN_WIDTH)
    , mCurrentTetronimo { 0, 0, 0, 0 }
    , mGameBoard { 0, 0, 0, 0 }
    , mFactory { mTextures }
    , mCollisionHandler { nullptr, nullptr, 0 } 
{
};

bool TetrisGameEngine::loadMedia()
{
    bool success = true;
    mTextures.resize(BLOCK_TEXTURE_TOTAL + 1);

    // Open the block textures
    success = success && loadTexture(BLOCK_TEXTURE_RED, "red.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_BLUE, "blue.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_YELLOW, "yellow.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_GREEN, "green.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_PURPLE, "purple.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_ORANGE, "orange.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_NAVY, "navy.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_GREY, "grey.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_WHITE, "white.bmp");
    success = success && loadTexture(BLOCK_TEXTURE_BLACK, "black.bmp");

    // Open the font
    std::string fontPath { std::string(ASSETS_DIR) + "/" + "Arial.ttf" };
    mFont = TTF_OpenFont(fontPath.c_str(), FONT_SIZE);
    if (mFont == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    return success;
}

bool TetrisGameEngine::create()
{
    mGameBoard = Grid(0, 0, N_ROWS, N_COLS);
    mGameBoard.updatePositions();
    mCollisionHandler = CollisionHandler(
        &mTextures[BLOCK_TEXTURE_WHITE],
        &mTextures[BLOCK_TEXTURE_BLACK],
        mElapsedTime);
    mCurrentTetronimo = mFactory.getNextTetronimo();
    return true;
}

bool TetrisGameEngine::update()
{
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
    SDL_SetRenderDrawColor(mRenderer, 0xC8, 0xC8, 0xC8, 0xFF);
    SDL_RenderDrawLine(mRenderer, 0, START_LINE, mScreenWidth, START_LINE);

    // Render game state objects
    mGameBoard.render();
    mCurrentTetronimo.render();
    mInfoBar.render(0, mScreenHeight - BOTTOM_BAR_HEIGHT);
    return true;
}