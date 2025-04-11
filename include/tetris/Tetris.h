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


// The game engine class
class GameEngine
{
public:
    GameEngine();
    virtual ~GameEngine();

    // Entry point. Run the game
    int run(int argc, char* args[]);

protected:
    // Starts up SDL and creates window
    bool init();

    // Concrete class implement these
    virtual bool loadMedia() = 0;
    virtual bool create() = 0;
    virtual bool update() = 0;
    virtual bool render() = 0;

    // Loads the texture at the file path
    bool loadTexture(const int, const std::string&);

    // Updates the information bar texture text
    void updateInformationBar();

    // Frees media and shuts down SDL
    void close();

    // The window we'll be rendering to
    SDL_Window* mWindow;

    // The window renderer
    SDL_Renderer* mRenderer;

    // textures and fonts
    Texture mTextures[BLOCK_TEXTURE_TOTAL];
    TTF_Font* mFont;

    // A text texture displaying FPS, score, etc
    Texture mInfoBar;
    std::stringstream mInfoText;

    // Event handling
    SDL_Event mEvent;

    // States
    bool mQuit;     // exit the actual game window altogether
    bool mPlaying;  // the playable part of the game is running or not

    // Counters
    Uint32 mElapsedTime;
    Uint32 mFrameCount;
    Uint32 mScore;
    int mFps;
};

#endif

class TetrisGameEngine : public GameEngine
{
public:
    TetrisGameEngine(); 

private:
    bool loadMedia() override;
    bool create() override;
    bool update() override;
    bool render() override;

    Grid mCurrentTetronimo;
    Grid mGameBoard;
    TetronimoFactory mFactory;
    CollisionHandler mCollisionHandler;
};