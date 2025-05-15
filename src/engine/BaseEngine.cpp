#include <iostream>

#include "engine/BaseEngine.h"
#include <cassert>
#include <memory>

BaseEngine::BaseEngine(const int screenHeight, const int screenWidth)
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

BaseEngine::~BaseEngine()
{
}

bool BaseEngine::init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    printf("Initialising SDL\n");
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
        printf("Creating SDL window\n");
        mWindow.reset(SDL_CreateWindow("Tetris",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            mScreenWidth,
            mScreenHeight,
            SDL_WINDOW_SHOWN));
        if (mWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create vsynced renderer for window
            printf("Creating SDL renderer\n");
            mRenderer.reset(SDL_CreateRenderer(
                mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
            if (mRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n",
                    SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(mRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                printf("Initialising SDL Image\n");
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n",
                        IMG_GetError());
                    success = false;
                }

                // Initialize SDL_ttf
                printf("Initialising SDL TTF\n");
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

bool BaseEngine::loadTexture(const std::string_view fileName)
{
    bool success = true;
    std::string filePath { std::string(ASSETS_DIR) + "/" + std::string(fileName) };
    printf("Loading %s\n", std::string(fileName).c_str());
    if (mTextures.find(fileName) != mTextures.end())
    {
        printf("Key %s has already been loaded to the textures map!\n", filePath.c_str());
        success = false;
    }
    else
    {
        mTextures.emplace(fileName, std::make_unique<Texture>(mRenderer.get()));
        if (!mTextures.at(fileName)->loadFromFile(filePath))
        {
            printf("Failed to load %s image!\n", filePath.c_str());
            success = false;
        }
    }
    return success;
}

bool BaseEngine::loadFont(const std::string_view fileName)
{
    bool success { true };
    std::string fontPath { std::string(ASSETS_DIR) + "/" + std::string(fileName) };
    printf("Loading %s\n", std::string(fileName).c_str());
    mFont.reset(TTF_OpenFont(fontPath.c_str(), FONT_SIZE));
    if (mFont == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    return success;
}

void BaseEngine::updateInformationBar()
{
    mInfoText.str("");
    mInfoText << "  fps  " << mFps << "  |  score  " << mScore;
    if (!mInfoBar->loadFromRenderedText(
            mInfoText.str().c_str(), TEXT_COLOUR, BACKGROUND_COLOUR))
    {
        printf("Failed to load text texture\n");
    }
}

void BaseEngine::close()
{
    // Free resrources
    mTextures.clear();
    mFont.reset();
    mRenderer.reset();
    mWindow.reset();

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int BaseEngine::run(int argc, char* args[])
{
    // Start up SDL and create window
    printf("Initialising engine\n");
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Load media
        printf("Loading media\n");
        if (!loadMedia() || !loadFont(FONT_ARIAL))
        {
            printf("Failed to load media!\n");
        }
        else
        {
            // Initialize the information bar text(ure)
            printf("Initialising information bar\n");
            mInfoBar = std::make_unique<Texture>(mRenderer.get(), mFont.get());
            updateInformationBar();
            mInfoText.str("");
            mElapsedTime = SDL_GetTicks();

            // Create the game state objects
            printf("Creating game state objects\n");
            create();

            // While application is running
            printf("Starting engine loop\n");
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
                SDL_SetRenderDrawColor(mRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(mRenderer.get());

                // Draw the information box
                SDL_SetRenderDrawColor(mRenderer.get(),
                    BACKGROUND_COLOUR.r,
                    BACKGROUND_COLOUR.g,
                    BACKGROUND_COLOUR.b,
                    BACKGROUND_COLOUR.a);
                static SDL_Rect infoBoxRect = {
                    0, mScreenHeight - BOTTOM_BAR_HEIGHT, mScreenWidth, BOTTOM_BAR_HEIGHT
                };
                SDL_RenderFillRect(mRenderer.get(), &infoBoxRect);

                // Render game state objects
                render();

                // Update screen
                SDL_RenderPresent(mRenderer.get());
            }
        }
    }

    // Free resources and close SDL
    printf("Shutting down engine\n");
    close();
    std::cout << "Finished cleanly" << '\n';
    return 0;
}