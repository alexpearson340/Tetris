#include "tetris/Tetris.h"
#include <iostream>

GameEngine::GameEngine()
    : mWindow { nullptr }
    , mRenderer { nullptr }
    , mInfoBar {}
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
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
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

bool GameEngine::loadMedia()
{
    bool success = true;

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

bool GameEngine::loadTexture(const int textureIndex, const std::string& fileName)
{
    bool success = true;
    std::string filePath { std::string(ASSETS_DIR) + "/" + fileName };
    Texture texture { mRenderer };
    gTextures[textureIndex] = texture;
    if (!gTextures[textureIndex].loadFromFile(filePath))
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
    for (int i = 0; i < BLOCK_TEXTURE_TOTAL; ++i)
    {
        gTextures[i].free();
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

            // Main loop flag
            bool quit = false;
            bool keepPlaying = true;

            // Event handler
            SDL_Event e;

            // Game state
            mElapsedTime = SDL_GetTicks();
            mGameBoard = Grid(0, 0, N_ROWS, N_COLS);
            mGameBoard.updatePositions();
            mCollisionHandler = CollisionHandler(&gTextures[BLOCK_TEXTURE_WHITE], &gTextures[BLOCK_TEXTURE_BLACK], mElapsedTime);
            Grid tetronimo = mFactory.getNextTetronimo();

            // While application is running
            while (!quit)
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

                // Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    if (keepPlaying)
                    {
                        // Handle input for the block
                        tetronimo.handleEvent(e);
                    }
                }

                if (keepPlaying)
                {
                    // Handle movement and collisions
                    if (mCollisionHandler.handle(tetronimo, mGameBoard, SDL_GetTicks()))
                    {
                        tetronimo = mFactory.getNextTetronimo();
                        mScore = mScore + 4;
                        updateInformationBar();
                    };
                    keepPlaying = mCollisionHandler.keepPlaying();
                }

                // Clear screen
                SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(mRenderer);

                // Draw the start line
                SDL_SetRenderDrawColor(mRenderer, 0xC8, 0xC8, 0xC8, 0xFF);
                SDL_RenderDrawLine(mRenderer, 0, START_LINE, SCREEN_WIDTH, START_LINE);

                // Draw the information box
                SDL_SetRenderDrawColor(mRenderer,
                    BACKGROUND_COLOUR.r,
                    BACKGROUND_COLOUR.g,
                    BACKGROUND_COLOUR.b,
                    BACKGROUND_COLOUR.a);
                static SDL_Rect infoBoxRect = {
                    0, BOTTOM_BAR_START, SCREEN_WIDTH, BOTTOM_BAR_HEIGHT
                };
                SDL_RenderFillRect(mRenderer, &infoBoxRect);

                // Render objects
                mGameBoard.render();
                tetronimo.render();
                mInfoBar.render(0, BOTTOM_BAR_START);

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