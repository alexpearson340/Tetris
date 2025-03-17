#include <iostream>
#include "Tetris.h"

// Different textures for blocks
enum BlockTextures
{
	BLOCK_TEXTURE_RED,
	BLOCK_TEXTURE_BLUE,
	BLOCK_TEXTURE_YELLOW,
	BLOCK_TEXTURE_GREEN,
	BLOCK_TEXTURE_PURPLE,
	BLOCK_TEXTURE_ORANGE,
    BLOCK_TEXTURE_NAVY,
    BLOCK_TEXTURE_GREY,
    BLOCK_TEXTURE_WHITE,
    BLOCK_TEXTURE_BLACK,
	BLOCK_TEXTURE_TOTAL,
};

//Container for all used Textures
Texture gTextures[ BLOCK_TEXTURE_TOTAL ];

// Implementations for Block, Grid, TetronimoFactory, CollisionHandler, and GameEngine
Block::Block() :
    mPosX(0),
    mPosY(0),
    mTexture(nullptr)
{
}

Block::Block(int x, int y, Texture* texture) :
    mPosX(x),
    mPosY(y),
    mTexture(texture)
{
}

void Block::move(int velX, int velY)
{
    mPosX += velX;
    mPosY += velY;
}

void Block::moveTo(int x, int y)
{
    mPosX = x;
    mPosY = y;
}

int Block::getPosX()
{
    return mPosX;
}

int Block::getPosY()
{
    return mPosY;
}

void Block::render()
{
    //Show the block
	mTexture->render( mPosX, mPosY );
}

Texture * Block::getTexture()
{
    return mTexture;
}

void Block::setTexture(Texture * texture)
{
    mTexture = texture;
}

bool Block::exists()
{
    return mTexture != nullptr;
}

Grid::Grid(int x, int y, size_t rows, size_t cols) : 
    mPosX(x),
    mPosY(y),
    mRows(rows),
    mCols(cols),
    mGrid(rows, std::vector<Block>(cols))
{
}

void Grid::createBlock(int i, int j, Texture* texture)
{
    int blockX = mPosX + (j * BLOCK_SIZE);
    int blockY = mPosY + (i * BLOCK_SIZE); 
    mGrid[i][j] = Block { blockX, blockY, texture };
};

Block & Grid::getBlock(int i, int j)
{
    return mGrid[i][j];
}

void Grid::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_DOWN: mVelY = VERTICAL_FAST_VELOCITY; break;
            case SDLK_LEFT: mVelX = -1 * BLOCK_SIZE; break;
            case SDLK_RIGHT: mVelX = BLOCK_SIZE; break;
            case SDLK_SPACE: mRotate = true; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_DOWN: mVelY = VERTICAL_VELOCITY; break;
            case SDLK_LEFT: mVelX = 0; break;
            case SDLK_RIGHT: mVelX = 0; break;
        }
    }
}

void Grid::move(int xMul, int yMul)
{
    bool allowedMove = true;
    int xMove = mVelX * xMul;
    int yMove = mVelY * yMul;

    //Move all the blocks
    for (int i = 0; i < mRows; ++i) 
    {
        for (int j = 0; j < mCols; ++j) 
        {
            mGrid[i][j].move(xMove, yMove);
        }
    }

    //Move the Grid ref along with the blocks
    mPosX += xMove;
    mPosY += yMove;
}

void Grid::rotateClockwise()
{
    transpose();
    // Reverse each row
    for (size_t i = 0; i < mRows; ++i) 
    {
        std::reverse(mGrid[i].begin(), mGrid[i].end());
    }
    updatePositions();
    mRotate = false;
}

void Grid::rotateAntiClockwise()
{   
    rotateClockwise();
    rotateClockwise();
    rotateClockwise();
}

void Grid::transpose()
{
    for (int i = 0; i < mRows; ++i) 
    {
        for (int j = i + 1; j < mRows; ++j) 
        {
            std::swap(mGrid[i][j], mGrid[j][i]);
        }
    }
}

void Grid::updatePositions()
{
    for (int i = 0; i < mRows; ++i) 
    {
        for (int j = 0; j < mCols; ++j) 
        {
            mGrid[i][j].moveTo(mPosX + ( j * BLOCK_SIZE ), mPosY + ( i * BLOCK_SIZE ) );
        }
    }
}

void Grid::moveRowsDown(int bottomRow, int nRowsToDelete)
{
    for (int r = bottomRow; r >= nRowsToDelete; r--)
    {
        mGrid[r] = mGrid[r - nRowsToDelete];
    }
}

void Grid::render()
{
    for (int i = 0; i < mRows; ++i) 
    {
        for (int j = 0; j < mCols; ++j) 
        {
            if (mGrid[i][j].exists())
            {
                mGrid[i][j].render();
            };
        }
    }
}

int Grid::getHeight()
{
    return mRows;
}

int Grid::getWidth()
{
    return mCols;
}

int Grid::getPosX()
{
    return mPosX;
}

int Grid::getPosY()
{
    return mPosY;
}

bool Grid::shouldRotate()
{
    return mRotate;
}

TetronimoFactory::TetronimoFactory()
{
    setup();
}

void TetronimoFactory::setup()
{
    // Seed the random number generator
    std::random_device rd;
    mGen = std::mt19937(rd());
    mDis = std::uniform_int_distribution<>(0, BLOCK_TEXTURE_NAVY);
}

Grid TetronimoFactory::getNextTetronimo()
{
    int randomNumber { mDis(mGen) };
    Grid grid{ mTetronimoStartX, mTetronimoStartY, 0, 0 };
    switch (randomNumber)
    {
        case BLOCK_TEXTURE_RED:
            grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
            grid.createBlock(0, 0, &gTextures[BLOCK_TEXTURE_RED]);
            grid.createBlock(0, 1, &gTextures[BLOCK_TEXTURE_RED]);
            grid.createBlock(1, 1, &gTextures[BLOCK_TEXTURE_RED]);
            grid.createBlock(1, 2, &gTextures[BLOCK_TEXTURE_RED]);
            break;

        case BLOCK_TEXTURE_BLUE:
            grid = Grid { mTetronimoStartX, mTetronimoStartY, 4, 4 };
            grid.createBlock(0, 1, &gTextures[BLOCK_TEXTURE_BLUE]);
            grid.createBlock(1, 1, &gTextures[BLOCK_TEXTURE_BLUE]);
            grid.createBlock(2, 1, &gTextures[BLOCK_TEXTURE_BLUE]);
            grid.createBlock(3, 1, &gTextures[BLOCK_TEXTURE_BLUE]);
            break;
        
        case BLOCK_TEXTURE_YELLOW:
            grid = Grid { mTetronimoStartX, mTetronimoStartY, 2, 2 };
            grid.createBlock(0, 0, &gTextures[BLOCK_TEXTURE_YELLOW]);
            grid.createBlock(0, 1, &gTextures[BLOCK_TEXTURE_YELLOW]);
            grid.createBlock(1, 0, &gTextures[BLOCK_TEXTURE_YELLOW]);
            grid.createBlock(1, 1, &gTextures[BLOCK_TEXTURE_YELLOW]);
            break;

        case BLOCK_TEXTURE_GREEN:
            grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
            grid.createBlock(0, 1, &gTextures[BLOCK_TEXTURE_GREEN]);
            grid.createBlock(0, 2, &gTextures[BLOCK_TEXTURE_GREEN]);
            grid.createBlock(1, 0, &gTextures[BLOCK_TEXTURE_GREEN]);
            grid.createBlock(1, 1, &gTextures[BLOCK_TEXTURE_GREEN]);
            break;

        case BLOCK_TEXTURE_PURPLE:
            grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
            grid.createBlock(0, 1, &gTextures[BLOCK_TEXTURE_PURPLE]);
            grid.createBlock(1, 0, &gTextures[BLOCK_TEXTURE_PURPLE]);
            grid.createBlock(1, 1, &gTextures[BLOCK_TEXTURE_PURPLE]);
            grid.createBlock(1, 2, &gTextures[BLOCK_TEXTURE_PURPLE]);
            break;

        case BLOCK_TEXTURE_ORANGE:
            grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
            grid.createBlock(0, 2, &gTextures[BLOCK_TEXTURE_ORANGE]);
            grid.createBlock(1, 0, &gTextures[BLOCK_TEXTURE_ORANGE]);
            grid.createBlock(1, 1, &gTextures[BLOCK_TEXTURE_ORANGE]);
            grid.createBlock(1, 2, &gTextures[BLOCK_TEXTURE_ORANGE]);
            break;

        case BLOCK_TEXTURE_NAVY:
            grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
            grid.createBlock(0, 0, &gTextures[BLOCK_TEXTURE_NAVY]);
            grid.createBlock(1, 0, &gTextures[BLOCK_TEXTURE_NAVY]);
            grid.createBlock(1, 1, &gTextures[BLOCK_TEXTURE_NAVY]);
            grid.createBlock(1, 2, &gTextures[BLOCK_TEXTURE_NAVY]);
            break;

        default:
            Grid grid { mTetronimoStartX, mTetronimoStartY, 0, 0 }; // Default initialization
            break;
    }

    return grid;
}

CollisionHandler::CollisionHandler(SDL_Renderer* renderer) :
    mPreviousTime { 0 },
    mCurrentTime { SDL_GetTicks() },
    mRenderer { renderer }
{

}

bool CollisionHandler::keepPlaying()
{
    return mKeepPlaying;
}

bool CollisionHandler::handle(Grid & tetronimo, Grid & gameBoard)
{   
    // Cap horizontal movements and rotations to a certain frequency
    mCurrentTime = SDL_GetTicks();
    if ((mCurrentTime - mPreviousTime) >= INPUT_INTERVAL_MS)
    {   
        mPreviousTime = mCurrentTime;
        handleHorizontal(tetronimo, gameBoard);
        handleRotational(tetronimo, gameBoard);
    }

    //Move the block vertically
    bool newTetronimoRequired { handleVertical(tetronimo, gameBoard) };
    return newTetronimoRequired;
}

void CollisionHandler::handleHorizontal(Grid & tetronimo, Grid & gameBoard)
{
    //Move the block horizontally
    tetronimo.move(1, 0);
    if (!checkCollisions(tetronimo, gameBoard))
    {
        tetronimo.move(-1, 0);
    };
}

void CollisionHandler::handleRotational(Grid & tetronimo, Grid & gameBoard)
{
    //Rotate the block
    if (tetronimo.shouldRotate())
    {
        tetronimo.rotateClockwise();
        if(!checkCollisions(tetronimo, gameBoard))
        {
            tetronimo.rotateAntiClockwise();
        }
    };
}

bool CollisionHandler::handleVertical(Grid & tetronimo, Grid & gameBoard)
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

void CollisionHandler::freezeTetronimo(Grid & tetronimo, Grid & gameBoard)
{   
    // find nearest whole number of blocks on game board
    int rowOnGameBoard = tetronimo.getPosY() / BLOCK_SIZE;
    int colOnGameBoard = tetronimo.getPosX() / BLOCK_SIZE;

    for (int i = 0; i < tetronimo.getHeight(); ++i) 
    {
        for (int j = 0; j < tetronimo.getWidth(); ++j) 
        {   
            Block & block = tetronimo.getBlock(i, j);
            if (block.exists())
            {   
                gameBoard.createBlock( rowOnGameBoard + i, colOnGameBoard + j, block.getTexture());
                if ( (rowOnGameBoard + i) * BLOCK_SIZE < START_LINE )
                {
                    mKeepPlaying = false;
                }
            }
        }
    }
}

void CollisionHandler::handleCompletedRows(Grid & tetronimo, Grid & gameBoard)
{
    // work out which rows have been completed
    int rowOnGameBoard = tetronimo.getPosY() / BLOCK_SIZE;
    int colOnGameBoard = tetronimo.getPosX() / BLOCK_SIZE;
    int rowNum;
    std::vector<int> completedRows;
    for (int i = 0; i < tetronimo.getHeight(); ++i)
    {   
        int rowNum = rowOnGameBoard + i;
        if ( (rowNum < gameBoard.getHeight()) && checkForCompletedRow(rowNum, gameBoard) )
        {
            completedRows.push_back(rowNum);
        }
    }

    if ( completedRows.size() > 0 )
    {   
        // make each completed row flash a little
        flashRows(completedRows, gameBoard, &gTextures[BLOCK_TEXTURE_BLACK]);
        flashRows(completedRows, gameBoard, &gTextures[BLOCK_TEXTURE_WHITE]);
        flashRows(completedRows, gameBoard, &gTextures[BLOCK_TEXTURE_BLACK]);
        flashRows(completedRows, gameBoard, &gTextures[BLOCK_TEXTURE_WHITE]);

        // delete the completed rows annd move existing rows down
        gameBoard.moveRowsDown(completedRows.back(), completedRows.size());

        // update positions
        gameBoard.updatePositions();
    }
}

bool CollisionHandler::checkForCompletedRow(int rowNum, Grid & gameBoard)
{
    for (int j = 0; j < gameBoard.getWidth(); ++j)
    {
        if ( !gameBoard.getBlock(rowNum, j).exists() )
        {
            return false;
        }
    }
    return true;
}

// Visual effect when the player completes a row
void CollisionHandler::flashRows(std::vector<int> completedRows, Grid & gameBoard, Texture * texture)
{
    for (int rowNum : completedRows) 
    {
        for (int j = 0; j < gameBoard.getWidth(); ++j)
        {
            gameBoard.getBlock(rowNum, j).setTexture(texture);
        }
    }

    // Update the screen just for this
    SDL_Delay(COMPLETED_ROW_FLASH_INTERVAL_MS);
    SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( mRenderer );
    gameBoard.render();
    SDL_RenderPresent( mRenderer );
}

bool CollisionHandler::checkCollisions(Grid & tetronimo, Grid & gameBoard)
{
    // find nearest whole number of blocks on game board
    int rowOnGameBoard = tetronimo.getPosY() / BLOCK_SIZE;
    int colOnGameBoard = tetronimo.getPosX() / BLOCK_SIZE;

    // bounday condition for the very bottom row
    rowOnGameBoard += ( rowOnGameBoard >= (gameBoard.getHeight() - tetronimo.getHeight())) ? 0 : 1; 

    for (int i = 0; i < tetronimo.getWidth(); ++i) 
    {
        for (int j = 0; j < tetronimo.getHeight(); ++j) 
        {   
            Block & block = tetronimo.getBlock(i, j);
            if (block.exists())
            {   
                //Check sides of screen
                if( ( block.getPosX() < 0) || (block.getPosX() + BLOCK_SIZE > SCREEN_WIDTH ) )
                {
                    return false;
                }
                //Check top and bottom of playable area
                if( ( block.getPosY() < 0) || (block.getPosY() + BLOCK_SIZE > (N_ROWS * BLOCK_SIZE) ) )
                {
                    return false;
                }
                //Check for an already frozen Tetronimo in the same position as the Block
                if( gameBoard.getBlock(rowOnGameBoard + i, colOnGameBoard + j).exists() )
                {
                    return false;
                }
            }
        }
    }
    return true;
}

GameEngine::GameEngine() :
    mWindow { nullptr },
    mRenderer { nullptr },
    mInfoBar {}
{
}

bool GameEngine::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		mWindow = SDL_CreateWindow( "Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( mWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( mRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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
    success = success && loadTexture( BLOCK_TEXTURE_RED, "assets/red.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_BLUE, "assets/blue.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_YELLOW, "assets/yellow.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_GREEN, "assets/green.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_PURPLE, "assets/purple.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_ORANGE, "assets/orange.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_NAVY, "assets/navy.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_GREY, "assets/grey.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_WHITE, "assets/white.bmp");
    success = success && loadTexture( BLOCK_TEXTURE_BLACK, "assets/black.bmp");

    //Open the font
	mFont = TTF_OpenFont( "assets/Arial.ttf", FONT_SIZE );
	if( mFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	return success;
}

bool GameEngine::loadTexture(const int textureIndex, const std::string & filePath)
{
    bool success = true; 
    Texture texture { mRenderer };
    gTextures[ textureIndex ] = texture;
    if( !gTextures[ textureIndex ].loadFromFile( filePath ) )
	{
		printf( "Failed to load %s image!\n", filePath.c_str() );
		success = false;
	}
    return success;
}

void GameEngine::updateInformationBar()
{   
    mInfoText.str( "" );
	mInfoText << "  fps  " << mFps << "  |  score  " << mScore;
    if ( !mInfoBar.loadFromRenderedText( mInfoText.str().c_str(), TEXT_COLOUR, BACKGROUND_COLOUR ) )
    {
        printf( "Failed to load text texture" );
    }
}

void GameEngine::close()
{
	//Free loaded images
	for( int i = 0; i < BLOCK_TEXTURE_TOTAL; ++i )
	{
		gTextures[ i ].free();
	}

    //Free global font
	TTF_CloseFont( mFont );
	mFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer( mRenderer );
	SDL_DestroyWindow( mWindow );
	mWindow = NULL;
	mRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int GameEngine::run( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
        if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
            //Initialize the information bar text(ure)
            mInfoBar = Texture(mRenderer, mFont);
            updateInformationBar();
            mInfoText.str( "" );

			//Main loop flag
			bool quit = false;
            bool keepPlaying = true;

			//Event handler
			SDL_Event e;

            // Game state
            mGameBoard = Grid(0, 0, N_ROWS, N_COLS);
            mGameBoard.updatePositions();
            mCollisionHandler = CollisionHandler(mRenderer);
            Grid tetronimo = mFactory.getNextTetronimo();

            mElapsedTime = SDL_GetTicks();

			//While application is running
			while( !quit )
			{   
                //Increment counters
                mFrameCount++;
                if (SDL_GetTicks() - mElapsedTime > 1000)
                {
                    mFps = mFrameCount;
                    mFrameCount = 0;
                    mElapsedTime = SDL_GetTicks();
                    updateInformationBar();
                }
                 
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }

                    if ( keepPlaying )
                    {
                        //Handle input for the block
                        tetronimo.handleEvent( e );
                    }
                }

                if ( keepPlaying )
                {
                    //Handle movement and collisions
                    if ( mCollisionHandler.handle(tetronimo, mGameBoard) )
                    {
                        tetronimo = mFactory.getNextTetronimo();
                        mScore = mScore + 4;
                        updateInformationBar();
                    };
                    keepPlaying = mCollisionHandler.keepPlaying();
                }

                //Clear screen
                SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( mRenderer );

                // Draw the start line
                SDL_SetRenderDrawColor( mRenderer, 0xC8, 0xC8, 0xC8, 0xFF);
                SDL_RenderDrawLine( mRenderer, 0, START_LINE, SCREEN_WIDTH, START_LINE );

                // Draw the information box
                SDL_SetRenderDrawColor( mRenderer, BACKGROUND_COLOUR.r, BACKGROUND_COLOUR.g, BACKGROUND_COLOUR.b, BACKGROUND_COLOUR.a);
                static SDL_Rect infoBoxRect = { 0, BOTTOM_BAR_START, SCREEN_WIDTH, BOTTOM_BAR_HEIGHT };
                SDL_RenderFillRect(mRenderer, &infoBoxRect);

                //Render objects
                mGameBoard.render();
                tetronimo.render();
                mInfoBar.render(0, BOTTOM_BAR_START);

                //Update screen
                SDL_RenderPresent( mRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();
    std::cout << "Finished cleanly" << '\n';
	return 0;
}