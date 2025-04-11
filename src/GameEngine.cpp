#include <iostream>

#include "tetris/GameEngine.h"

GameEngine::GameEngine(const int screenHeight, const int screenWidth)
    : mScreenHeight { screenHeight }
    , mScreenWidth { screenWidth }
    , mWindow { nullptr }
    , mRenderer { nullptr }
    , mFont { nullptr }
    , mInfoBar {}
    , mInfoText {}
    , mQuit { false }
    , mPlaying { true }
    , mElapsedTime { 0 }
    , mFrameCount { 0 }
    , mScore { 0 }
    , mFps { 0 }
{
}

GameEngine::~GameEngine()
{
}

bool GameEngine::init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        mWindow = SDL_CreateWindow("Tetris",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            mScreenWidth,
            mScreenHeight,
            SDL_WINDOW_SHOWN);
        if (mWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create vsynced renderer for window
            mRenderer = SDL_CreateRenderer(
                mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (mRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n",
                    SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n",
                        IMG_GetError());
                    success = false;
                }

                // Initialize SDL_ttf
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                        TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool GameEngine::loadTexture(const int textureIndex, const std::string& fileName)
{
    bool success = true;
    std::string filePath { std::string(ASSETS_DIR) + "/" + fileName };
    Texture texture { mRenderer };
    mTextures[textureIndex] = texture;
    if (!mTextures[textureIndex].loadFromFile(filePath))
    {
        printf("Failed to load %s image!\n", filePath.c_str());
        success = false;
    }
    return success;
}

void GameEngine::updateInformationBar()
{
    mInfoText.str("");
    mInfoText << "  fps  " << mFps << "  |  score  " << mScore;
    if (!mInfoBar.loadFromRenderedText(
            mInfoText.str().c_str(), TEXT_COLOUR, BACKGROUND_COLOUR))
    {
        printf("Failed to load text texture");
    }
}

void GameEngine::close()
{
    // Free loaded images
    for (int i = 0; i < mTextures.size(); ++i)
    {
        mTextures[i].free();
    }

    // Free global font
    TTF_CloseFont(mFont);
    mFont = NULL;

    // Destroy window
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
    mRenderer = NULL;

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int GameEngine::run(int argc, char* args[])
{
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            // Initialize the information bar text(ure)
            mInfoBar = Texture(mRenderer, mFont);
            updateInformationBar();
            mInfoText.str("");
            mElapsedTime = SDL_GetTicks();

            // Create the game state objects
            create();

            // While application is running
            while (!mQuit)
            {
                // Increment counters
                mFrameCount++;
                if (SDL_GetTicks() - mElapsedTime > 1000)
                {
                    mFps = mFrameCount;
                    mFrameCount = 0;
                    mElapsedTime = SDL_GetTicks();
                    updateInformationBar();
                }

                // Update game state objects
                update();

                // Clear screen
                SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(mRenderer);

                // Draw the information box
                SDL_SetRenderDrawColor(mRenderer,
                    BACKGROUND_COLOUR.r,
                    BACKGROUND_COLOUR.g,
                    BACKGROUND_COLOUR.b,
                    BACKGROUND_COLOUR.a);
                static SDL_Rect infoBoxRect = {
                    0, mScreenHeight - BOTTOM_BAR_HEIGHT, mScreenWidth, BOTTOM_BAR_HEIGHT
                };
                SDL_RenderFillRect(mRenderer, &infoBoxRect);

                // Render game state objects
                render();

                // Update screen
                SDL_RenderPresent(mRenderer);
            }
        }
    }

    // Free resources and close SDL
    close();
    std::cout << "Finished cleanly" << '\n';
    return 0;
}