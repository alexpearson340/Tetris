#ifndef TETRIS_H
#define TETRIS_H

#include "Block.h"
#include "CollisionHandler.h"
#include "Constants.h"
#include "Grid.h"
#include "TetronimoFactory.h"
#include "Texture.h"
#include <random>
#include <sstream>

// Container for all used Textures
inline Texture gTextures[BLOCK_TEXTURE_TOTAL];


// The game engine class
class GameEngine
{
public:
    GameEngine();

    // Entry point. Run the game
    int run(int argc, char* args[]);

private:
    // Starts up SDL and creates window
    bool init();

    // Loads media
    bool loadMedia();
    bool loadTexture(const int, const std::string&);

    // Updates the information bar texture text
    void updateInformationBar();

    // Frees media and shuts down SDL
    void close();

    // Game state classes
    Grid mGameBoard { 0, 0, 0, 0 };
    TetronimoFactory mFactory { gTextures };
    CollisionHandler mCollisionHandler { nullptr, nullptr, 0 };

    // The window we'll be rendering to
    SDL_Window* mWindow = nullptr;

    // The window renderer
    SDL_Renderer* mRenderer = nullptr;

    // The global font we are using for text
    TTF_Font* mFont = nullptr;

    // A text texture displaying FPS, score, etc
    Texture mInfoBar {};
    std::stringstream mInfoText {};

    // Counters
    Uint32 mElapsedTime = 0;
    Uint32 mFrameCount = 0;
    Uint32 mScore = 0;
    int mFps = 0;
};

#endif