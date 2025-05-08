#ifndef BASEENGINE_H
#define BASEENGINE_H

#include "engine/Texture.h"
#include <sstream>

inline constexpr SDL_Color BACKGROUND_COLOUR { 250, 250, 250, 255 };
inline constexpr SDL_Color TEXT_COLOUR { 0, 0, 0, 255 };
inline constexpr int BOTTOM_BAR_HEIGHT { 24 };
inline constexpr int FONT_SIZE = 18;

// The game engine class
class BaseEngine
{
public:
    BaseEngine(int, int);
    virtual ~BaseEngine();

    // We dont support copy constructor, copy assignment, move constructor, move assignment
    BaseEngine(const BaseEngine&) = delete;
    BaseEngine& operator=(const BaseEngine&) = delete;
    BaseEngine(BaseEngine&&) = delete;
    BaseEngine& operator=(BaseEngine&&) = delete;

    // Entry point. Run the game
    int run(int argc, char* args[]);

protected:

    const int mScreenHeight;
    const int mScreenWidth;

    // Starts up SDL and creates window
    bool init();

    // Concrete class implement these
    virtual bool loadMedia() = 0;
    virtual bool create() = 0;
    virtual bool update() = 0;
    virtual bool render() = 0;

    // Loads the textures at the file path
    bool loadTexture(const std::string_view);
    bool loadFont(const std::string_view);

    // Updates the information bar texture text
    void updateInformationBar();

    // Frees media and shuts down SDL
    void close();

    // The window we'll be rendering to
    SDL_Window* mWindow;

    // The window renderer
    SDL_Renderer* mRenderer;

    // textures and fonts
    std::unordered_map<std::string_view, Texture> mTextures;
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