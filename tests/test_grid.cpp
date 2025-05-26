#include "tetris/Grid.h"
#include "engine/Texture.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

// Test fixture for Grid tests
class GridTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create mock textures
        mockTexture1 = std::make_unique<Texture>();
        mockTexture2 = std::make_unique<Texture>();
        
        // Create a simple 4x4 test grid
        testGrid = std::make_unique<Grid>(0, 0, 4, 4);
    }
    
    // Helper function to count existing blocks in grid
    int countExistingBlocks(Grid& grid)
    {
        int count = 0;
        grid.forEachBlock([&count](Block& block, size_t, size_t) {
            if (block.exists()) {
                count++;
            }
        });
        return count;
    }
    
    // Helper to create a simple 2x2 square tetromino pattern
    void createSquareTetromino(Grid& grid)
    {
        grid.createBlock(0, 0, mockTexture1.get());
        grid.createBlock(1, 0, mockTexture1.get());
        grid.createBlock(0, 1, mockTexture1.get());
        grid.createBlock(1, 1, mockTexture1.get());
    }
    
    std::unique_ptr<Texture> mockTexture1;
    std::unique_ptr<Texture> mockTexture2;
    std::unique_ptr<Grid> testGrid;
};

// Test constructor
TEST_F(GridTest, Constructor)
{
    Grid grid(100, 200, 5, 6);
    
    EXPECT_EQ(grid.getPosX(), 100);
    EXPECT_EQ(grid.getPosY(), 200);
    EXPECT_EQ(grid.getHeight(), 5);
    EXPECT_EQ(grid.getWidth(), 6);
}

// Test constructor initializes empty grid
TEST_F(GridTest, ConstructorCreatesEmptyGrid)
{
    Grid grid(0, 0, 3, 3);
    
    int existingBlocks = countExistingBlocks(grid);
    EXPECT_EQ(existingBlocks, 0);
}

// Test createBlock
TEST_F(GridTest, CreateBlock)
{
    testGrid->createBlock(1, 2, mockTexture1.get());
    
    Block& block = testGrid->getBlock(1, 2);
    EXPECT_TRUE(block.exists());
    EXPECT_EQ(block.getTexture(), mockTexture1.get());
    EXPECT_EQ(block.getPosX(), 0 + 1 * BLOCK_SIZE);
    EXPECT_EQ(block.getPosY(), 0 + 2 * BLOCK_SIZE);
}

// Test createBlock with grid offset
TEST_F(GridTest, CreateBlockWithGridOffset)
{
    Grid grid(100, 200, 4, 4);
    grid.createBlock(1, 2, mockTexture1.get());
    
    Block& block = grid.getBlock(1, 2);
    EXPECT_EQ(block.getPosX(), 100 + 1 * BLOCK_SIZE);
    EXPECT_EQ(block.getPosY(), 200 + 2 * BLOCK_SIZE);
}

// Test getBlock
TEST_F(GridTest, GetBlock)
{
    testGrid->createBlock(2, 3, mockTexture1.get());
    
    Block& block = testGrid->getBlock(2, 3);
    EXPECT_TRUE(block.exists());
    
    // Test getting empty block
    Block& emptyBlock = testGrid->getBlock(0, 0);
    EXPECT_FALSE(emptyBlock.exists());
}

// Test forEachBlock
TEST_F(GridTest, ForEachBlock)
{
    // Create a pattern
    testGrid->createBlock(0, 0, mockTexture1.get());
    testGrid->createBlock(1, 1, mockTexture1.get());
    testGrid->createBlock(2, 2, mockTexture1.get());
    
    int count = 0;
    std::vector<std::pair<size_t, size_t>> positions;
    
    testGrid->forEachBlock([&](Block& block, size_t x, size_t y) {
        if (block.exists()) {
            count++;
            positions.push_back({x, y});
        }
    });
    
    EXPECT_EQ(count, 3);
    EXPECT_EQ(positions.size(), 3);
    // Verify we found the right positions
    EXPECT_TRUE(std::find(positions.begin(), positions.end(), 
                         std::make_pair(size_t(0), size_t(0))) != positions.end());
    EXPECT_TRUE(std::find(positions.begin(), positions.end(), 
                         std::make_pair(size_t(1), size_t(1))) != positions.end());
    EXPECT_TRUE(std::find(positions.begin(), positions.end(), 
                         std::make_pair(size_t(2), size_t(2))) != positions.end());
}

// Test anyBlocks
TEST_F(GridTest, AnyBlocksTrue)
{
    testGrid->createBlock(1, 1, mockTexture1.get());
    
    bool hasBlockAt11 = testGrid->anyBlocks([](Block& block, size_t x, size_t y) {
        return block.exists() && x == 1 && y == 1;
    });
    
    EXPECT_TRUE(hasBlockAt11);
}

TEST_F(GridTest, AnyBlocksFalse)
{
    testGrid->createBlock(1, 1, mockTexture1.get());
    
    bool hasBlockAt00 = testGrid->anyBlocks([](Block& block, size_t x, size_t y) {
        return block.exists() && x == 0 && y == 0;
    });
    
    EXPECT_FALSE(hasBlockAt00);
}

// Test move
TEST_F(GridTest, MoveGrid)
{
    createSquareTetromino(*testGrid);
    
    // Set velocity and move right
    testGrid->setVelX(BLOCK_SIZE);
    testGrid->move(1, 0);
    
    EXPECT_EQ(testGrid->getPosX(), BLOCK_SIZE);
    EXPECT_EQ(testGrid->getPosY(), 0);
    
    // Check that blocks moved too
    Block& block = testGrid->getBlock(0, 0);
    EXPECT_EQ(block.getPosX(), BLOCK_SIZE);
}

TEST_F(GridTest, MoveGridVertically)
{
    createSquareTetromino(*testGrid);
    
    // Set velocity and move down
    testGrid->setVelY(VERTICAL_VELOCITY);
    testGrid->move(0, 1);
    
    EXPECT_EQ(testGrid->getPosX(), 0);
    EXPECT_EQ(testGrid->getPosY(), VERTICAL_VELOCITY);
    
    // Check that blocks moved too
    Block& block = testGrid->getBlock(0, 0);
    EXPECT_EQ(block.getPosY(), VERTICAL_VELOCITY);
}

// Test rotation - clockwise
TEST_F(GridTest, RotateClockwise2x2)
{
    Grid grid(0, 0, 2, 2);
    // Create L-shape: X.
    //                 XX
    grid.createBlock(0, 0, mockTexture1.get());
    grid.createBlock(0, 1, mockTexture1.get());
    grid.createBlock(1, 1, mockTexture1.get());
    
    grid.rotateClockwise();
    
    // After rotation: XX
    //                 X.
    EXPECT_TRUE(grid.getBlock(0, 0).exists());
    EXPECT_TRUE(grid.getBlock(1, 0).exists());
    EXPECT_TRUE(grid.getBlock(0, 1).exists());
    EXPECT_FALSE(grid.getBlock(1, 1).exists());
}

TEST_F(GridTest, RotateClockwise3x3)
{
    Grid grid(0, 0, 3, 3);
    // Create T-shape: .X.
    //                 XXX
    //                 ...
    grid.createBlock(1, 0, mockTexture1.get());
    grid.createBlock(0, 1, mockTexture1.get());
    grid.createBlock(1, 1, mockTexture1.get());
    grid.createBlock(2, 1, mockTexture1.get());
    
    grid.rotateClockwise();
    
    // After rotation: .X.
    //                 .XX
    //                 .X.
    EXPECT_TRUE(grid.getBlock(1, 0).exists());
    EXPECT_TRUE(grid.getBlock(1, 1).exists());
    EXPECT_TRUE(grid.getBlock(2, 1).exists());
    EXPECT_TRUE(grid.getBlock(1, 2).exists());
    EXPECT_EQ(countExistingBlocks(grid), 4);
}

// Test anticlockwise rotation
TEST_F(GridTest, RotateAntiClockwise)
{
    Grid grid(0, 0, 2, 2);
    grid.createBlock(0, 0, mockTexture1.get());
    grid.createBlock(1, 0, mockTexture1.get());
    
    grid.rotateAntiClockwise();
    
    // Should be rotated 90 degrees counter-clockwise
    EXPECT_TRUE(grid.getBlock(0, 0).exists());
    EXPECT_TRUE(grid.getBlock(0, 1).exists());
}

// Test updatePositions
TEST_F(GridTest, UpdatePositions)
{
    Grid grid(100, 200, 2, 2);
    grid.createBlock(0, 0, mockTexture1.get());
    grid.createBlock(1, 1, mockTexture1.get());
    
    // Set velocities before moving
    grid.setVelX(BLOCK_SIZE);
    grid.setVelY(VERTICAL_VELOCITY);
    
    // Move grid position
    grid.move(1, 1);
    
    // Now update positions to sync blocks with grid position
    grid.updatePositions();
    
    // Grid should have moved by velocities
    EXPECT_EQ(grid.getPosX(), 100 + BLOCK_SIZE);
    EXPECT_EQ(grid.getPosY(), 200 + VERTICAL_VELOCITY);
    
    // Blocks should be at grid position + their relative positions
    EXPECT_EQ(grid.getBlock(0, 0).getPosX(), 100 + BLOCK_SIZE);
    EXPECT_EQ(grid.getBlock(0, 0).getPosY(), 200 + VERTICAL_VELOCITY);
    EXPECT_EQ(grid.getBlock(1, 1).getPosX(), 100 + BLOCK_SIZE + BLOCK_SIZE);
    EXPECT_EQ(grid.getBlock(1, 1).getPosY(), 200 + VERTICAL_VELOCITY + BLOCK_SIZE);
}

// Test moveRowsDown
TEST_F(GridTest, MoveRowsDown)
{
    Grid grid(0, 0, 4, 4);
    // Create pattern in rows 0 and 1
    grid.createBlock(0, 0, mockTexture1.get());
    grid.createBlock(1, 0, mockTexture1.get());
    grid.createBlock(0, 1, mockTexture2.get());
    grid.createBlock(1, 1, mockTexture2.get());
    
    // Move rows down from row 3, deleting 1 row
    grid.moveRowsDown(3, 1);
    
    // Row 0 should now be empty
    EXPECT_FALSE(grid.getBlock(0, 0).exists());
    EXPECT_FALSE(grid.getBlock(1, 0).exists());
    
    // Original row 0 should now be at row 1
    EXPECT_TRUE(grid.getBlock(0, 1).exists());
    EXPECT_EQ(grid.getBlock(0, 1).getTexture(), mockTexture1.get());
    
    // Original row 1 should now be at row 2
    EXPECT_TRUE(grid.getBlock(0, 2).exists());
    EXPECT_EQ(grid.getBlock(0, 2).getTexture(), mockTexture2.get());
}

// Test event handling
TEST_F(GridTest, HandleEventKeyDown)
{
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    
    // Test DOWN key
    event.key.keysym.sym = SDLK_DOWN;
    testGrid->handleEvent(event);
    testGrid->move(0, 1);
    EXPECT_EQ(testGrid->getPosY(), VERTICAL_FAST_VELOCITY);
    
    // Reset
    testGrid = std::make_unique<Grid>(0, 0, 4, 4);
    
    // Test LEFT key
    event.key.keysym.sym = SDLK_LEFT;
    testGrid->handleEvent(event);
    testGrid->move(1, 0);
    EXPECT_EQ(testGrid->getPosX(), -BLOCK_SIZE);
    
    // Reset
    testGrid = std::make_unique<Grid>(0, 0, 4, 4);
    
    // Test RIGHT key
    event.key.keysym.sym = SDLK_RIGHT;
    testGrid->handleEvent(event);
    testGrid->move(1, 0);
    EXPECT_EQ(testGrid->getPosX(), BLOCK_SIZE);
    
    // Test SPACE key (rotation)
    event.key.keysym.sym = SDLK_SPACE;
    testGrid->handleEvent(event);
    EXPECT_TRUE(testGrid->shouldRotate());
}

TEST_F(GridTest, HandleEventKeyUp)
{
    SDL_Event event;
    event.type = SDL_KEYUP;
    
    // First set fast velocity
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_DOWN;
    testGrid->handleEvent(event);
    
    // Then release
    event.type = SDL_KEYUP;
    testGrid->handleEvent(event);
    testGrid->move(0, 1);
    EXPECT_EQ(testGrid->getPosY(), VERTICAL_VELOCITY);
}

// Test velocity setters
TEST_F(GridTest, SetVelocity)
{
    testGrid->setVelX(42);
    testGrid->setVelY(24);
    
    testGrid->move(1, 1);
    
    EXPECT_EQ(testGrid->getPosX(), 42);
    EXPECT_EQ(testGrid->getPosY(), 24);
}

// Test shouldRotate
TEST_F(GridTest, ShouldRotate)
{
    EXPECT_FALSE(testGrid->shouldRotate());
    
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_SPACE;
    testGrid->handleEvent(event);
    
    EXPECT_TRUE(testGrid->shouldRotate());
    
    // After rotation, should reset
    testGrid->rotateClockwise();
    EXPECT_FALSE(testGrid->shouldRotate());
}

// Test rotation preserves block textures
TEST_F(GridTest, RotationPreservesTextures)
{
    Grid grid(0, 0, 3, 3);
    grid.createBlock(0, 0, mockTexture1.get());
    grid.createBlock(1, 0, mockTexture2.get());
    
    grid.rotateClockwise();
    
    // Find the blocks with our textures
    int texture1Count = 0;
    int texture2Count = 0;
    
    grid.forEachBlock([&](Block& block, size_t, size_t) {
        if (block.exists()) {
            if (block.getTexture() == mockTexture1.get()) texture1Count++;
            if (block.getTexture() == mockTexture2.get()) texture2Count++;
        }
    });
    
    EXPECT_EQ(texture1Count, 1);
    EXPECT_EQ(texture2Count, 1);
}

// Edge case: Empty grid operations
TEST_F(GridTest, EmptyGridOperations)
{
    Grid emptyGrid(0, 0, 3, 3);
    
    // Should not crash
    ASSERT_NO_THROW(emptyGrid.move(1, 1));
    ASSERT_NO_THROW(emptyGrid.rotateClockwise());
    ASSERT_NO_THROW(emptyGrid.updatePositions());
    ASSERT_NO_THROW(emptyGrid.render());
    
    EXPECT_EQ(countExistingBlocks(emptyGrid), 0);
}