#include "tetris/CollisionHandler.h"
#include "engine/Texture.h"
#include <gtest/gtest.h>
#include <memory>

class CollisionHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        whiteTexture = std::make_unique<Texture>();
        blackTexture = std::make_unique<Texture>();
        blockTexture = std::make_unique<Texture>();
        
        currentTime = 0;
        handler = std::make_unique<CollisionHandler>(whiteTexture.get(), blackTexture.get(), currentTime);
        
        // Create empty game board
        gameBoard = std::make_unique<Grid>(0, 0, N_ROWS, N_COLS);
        
        // Create a simple 2x2 tetromino
        tetromino = std::make_unique<Grid>(160, 0, 2, 2);
        tetromino->createBlock(0, 0, blockTexture.get());
        tetromino->createBlock(1, 0, blockTexture.get());
        tetromino->createBlock(0, 1, blockTexture.get());
        tetromino->createBlock(1, 1, blockTexture.get());
    }
    
    std::unique_ptr<Texture> whiteTexture;
    std::unique_ptr<Texture> blackTexture;
    std::unique_ptr<Texture> blockTexture;
    std::unique_ptr<CollisionHandler> handler;
    std::unique_ptr<Grid> gameBoard;
    std::unique_ptr<Grid> tetromino;
    uint32_t currentTime;
};

TEST_F(CollisionHandlerTest, InitialState)
{
    EXPECT_TRUE(handler->keepPlaying());
}

TEST_F(CollisionHandlerTest, NoCollisionReturnsFalse)
{
    // Tetromino in open space should not trigger new tetromino
    bool needNewTetromino = handler->handle(*tetromino, *gameBoard, currentTime);
    EXPECT_FALSE(needNewTetromino);
    EXPECT_TRUE(handler->keepPlaying());
}

TEST_F(CollisionHandlerTest, TetrominoMovesDown)
{
    int startY = tetromino->getPosY();
    
    handler->handle(*tetromino, *gameBoard, currentTime);
    
    // Tetromino should have moved down
    EXPECT_GT(tetromino->getPosY(), startY);
}

TEST_F(CollisionHandlerTest, CollisionAtBottom)
{
    // Move tetromino near bottom
    tetromino = std::make_unique<Grid>(160, (N_ROWS - 2) * BLOCK_SIZE - 1, 2, 2);
    tetromino->createBlock(0, 0, blockTexture.get());
    tetromino->createBlock(1, 0, blockTexture.get());
    tetromino->createBlock(0, 1, blockTexture.get());
    tetromino->createBlock(1, 1, blockTexture.get());
    
    bool needNewTetromino = handler->handle(*tetromino, *gameBoard, currentTime);
    
    // Should detect collision and request new tetromino
    EXPECT_TRUE(needNewTetromino);
}

TEST_F(CollisionHandlerTest, HorizontalMovement)
{
    // Set horizontal velocity
    tetromino->setVelX(BLOCK_SIZE);
    
    // Wait enough time for input to be processed
    currentTime = INPUT_INTERVAL_MS + 1;
    
    int startX = tetromino->getPosX();
    handler->handle(*tetromino, *gameBoard, currentTime);
    
    // Should have moved right
    EXPECT_GT(tetromino->getPosX(), startX);
}

TEST_F(CollisionHandlerTest, WallCollisionLeft)
{
    // Place tetromino at left edge
    tetromino = std::make_unique<Grid>(0, 100, 2, 2);
    tetromino->createBlock(0, 0, blockTexture.get());
    tetromino->createBlock(1, 0, blockTexture.get());
    tetromino->createBlock(0, 1, blockTexture.get());
    tetromino->createBlock(1, 1, blockTexture.get());
    tetromino->setVelX(-BLOCK_SIZE);
    
    currentTime = INPUT_INTERVAL_MS + 1;
    
    handler->handle(*tetromino, *gameBoard, currentTime);
    
    // Should not move past left wall
    EXPECT_EQ(tetromino->getPosX(), 0);
}

TEST_F(CollisionHandlerTest, WallCollisionRight)
{
    // Place tetromino at right edge
    int rightEdge = SCREEN_WIDTH - 2 * BLOCK_SIZE;
    tetromino = std::make_unique<Grid>(rightEdge, 100, 2, 2);
    tetromino->createBlock(0, 0, blockTexture.get());
    tetromino->createBlock(1, 0, blockTexture.get());
    tetromino->createBlock(0, 1, blockTexture.get());
    tetromino->createBlock(1, 1, blockTexture.get());
    tetromino->setVelX(BLOCK_SIZE);
    
    currentTime = INPUT_INTERVAL_MS + 1;
    
    handler->handle(*tetromino, *gameBoard, currentTime);
    
    // Should not move past right wall
    EXPECT_EQ(tetromino->getPosX(), rightEdge);
}

TEST_F(CollisionHandlerTest, GameOverWhenBlocksReachTop)
{
    // Place a block at the top of the game board
    gameBoard->createBlock(5, 1, blockTexture.get());
    
    // Create tetromino that will collide with it
    tetromino = std::make_unique<Grid>(5 * BLOCK_SIZE, 0, 1, 1);
    tetromino->createBlock(0, 0, blockTexture.get());
    
    handler->handle(*tetromino, *gameBoard, currentTime);
    
    // Game should end
    EXPECT_FALSE(handler->keepPlaying());
}

TEST_F(CollisionHandlerTest, CollisionWithExistingBlocks)
{
    // Place some blocks on the game board
    gameBoard->createBlock(4, 10, blockTexture.get());
    gameBoard->createBlock(5, 10, blockTexture.get());
    
    // Create tetromino above them
    tetromino = std::make_unique<Grid>(4 * BLOCK_SIZE, 8 * BLOCK_SIZE, 2, 2);
    tetromino->createBlock(0, 0, blockTexture.get());
    tetromino->createBlock(1, 0, blockTexture.get());
    tetromino->createBlock(0, 1, blockTexture.get());
    tetromino->createBlock(1, 1, blockTexture.get());
    
    bool needNewTetromino = handler->handle(*tetromino, *gameBoard, currentTime);
    
    // Should eventually collide and freeze
    EXPECT_TRUE(needNewTetromino);
    
    // Check that blocks were added to game board
    EXPECT_TRUE(gameBoard->getBlock(4, 8).exists());
    EXPECT_TRUE(gameBoard->getBlock(5, 8).exists());
}