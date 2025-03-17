#ifndef TETRIS_H
#define TETRIS_H

#include <random>
#include "Texture.h"
#include <sstream>

// global constants
const int BLOCK_SIZE = 40;
const int VERTICAL_VELOCITY = 1;
const int VERTICAL_FAST_VELOCITY = 3 * VERTICAL_VELOCITY;

const int INPUT_INTERVAL_MS = 50;
const int COMPLETED_ROW_FLASH_INTERVAL_MS = 100;

const int N_ROWS = 22;
const int N_COLS = 10;
const int START_LINE = 2 * BLOCK_SIZE;              // N_ROWS - 2 is playable
const int BOTTOM_BAR_START = N_ROWS * BLOCK_SIZE;
const int BOTTOM_BAR_HEIGHT = 24;
const int SCREEN_WIDTH { BLOCK_SIZE * N_COLS };
const int SCREEN_HEIGHT { (BLOCK_SIZE * N_ROWS) + BOTTOM_BAR_HEIGHT };
const int TETRONIMO_START_X = 120;
const int TETRONIMO_START_Y = 0;

const int FONT_SIZE = 18;
const SDL_Color TEXT_COLOUR { 0, 0, 0, 255};
const SDL_Color BACKGROUND_COLOUR { 250, 250, 250, 255};

//The Blocks that will move around on the screen
class Block
{
    public:
        //Default constructor
        Block();

		//Initializes the variables
		Block(int, int, Texture*);

        //Move by an amount
		void move(int, int);

        //Move to a location
        void moveTo(int, int);

        int getPosX();

        int getPosY();

		//Shows the Block on the screen
		void render();

        //Having virtual blocks to fill unoccupied Grid squares is easier
        //than the handling required around std::optional<Block> in the Grid
        bool exists();

        Texture* getTexture();

        void setTexture(Texture *);

    private:
		//The X and Y offsets of the Block
		int mPosX, mPosY;

        // The texture associated with the block
        Texture* mTexture;
};

// A class wrapping a 2D grid of Blocks.
// The game board, as well as individual Tetronimos are
// stored in a Grid. Provides rotating functionality 
class Grid
{
    public:
        Grid(int, int, size_t, size_t);

        //Takes key presses and adjusts the Block's velocity
		void handleEvent( SDL_Event& e );

		void move(int, int);

        void createBlock(int, int, Texture*);

        Block & getBlock(int, int);

        void rotateClockwise();

        void rotateAntiClockwise();

        void render();

        int getHeight();

        int getWidth();

        int getPosX();

        int getPosY();

        bool shouldRotate();

        void updatePositions();

        void moveRowsDown(int, int);

    private:
        //The X and Y offsets of the grid
		int mPosX, mPosY;

        //The velocity of the grid
		int mVelX = 0;
        int mVelY = VERTICAL_VELOCITY;
        bool mRotate = false;   //The user has pressed the rotate key and it should rotate this frame

        void transpose();

        size_t mRows;
        size_t mCols;
        std::vector<std::vector<Block>> mGrid; // 2D vector of Blocks or nulls
};

// Provides a random Grid representing a Tetronimo on demand
class TetronimoFactory
{
    public:
        TetronimoFactory();

        Grid getNextTetronimo();
    
    private:
        void setup();
        std::mt19937 mGen; // Mersenne Twister generator
        std::uniform_int_distribution<> mDis; // Uniform distribution
        const int mTetronimoStartX {TETRONIMO_START_X};
        const int mTetronimoStartY {TETRONIMO_START_Y};
};

// Handles the horizontal, rotational and vertical
// collision scenarios, as well as freezing Tetronimos
// when they stop moving, and deleting rows which the 
// players had completed
class CollisionHandler
{
    public:
        CollisionHandler(SDL_Renderer*);
        
        bool handle(Grid &, Grid &);

        bool keepPlaying();

    private:
        void handleHorizontal(Grid &, Grid &);

        void handleRotational(Grid &, Grid &);

        bool handleVertical(Grid &, Grid &);

        void freezeTetronimo(Grid &, Grid &);

        void handleCompletedRows(Grid &, Grid &);

        bool checkForCompletedRow(int, Grid &);

        void flashRows(std::vector<int>, Grid &, Texture *);

        bool checkCollisions(Grid &, Grid &);

        bool mKeepPlaying = true;
        Uint32 mPreviousTime;
        Uint32 mCurrentTime;

        SDL_Renderer* mRenderer;
};

// The game engine class
class GameEngine
{   
    public:
        GameEngine();

        // Entry point. Run the game
        int run(int argc, char* args[]);

    private:
        //Starts up SDL and creates window
        bool init();

        //Loads media
        bool loadMedia();
        bool loadTexture(const int, const std::string &);

        //Updates the information bar texture text
        void updateInformationBar();

        //Frees media and shuts down SDL
        void close();

        // Game state classes
        Grid mGameBoard {0, 0, 0, 0};
        TetronimoFactory mFactory;
        CollisionHandler mCollisionHandler { nullptr };

        //The window we'll be rendering to
        SDL_Window* mWindow = nullptr;

        //The window renderer
        SDL_Renderer* mRenderer = nullptr;

        //The global font we are using for text
        TTF_Font* mFont = nullptr;

        //A text texture displaying FPS, score, etc
        Texture mInfoBar {};
        std::stringstream mInfoText {};

        //Counters
        Uint32 mElapsedTime = 0;
        Uint32 mFrameCount = 0;
        Uint32 mScore = 0;
        int mFps = 0;
};

#endif