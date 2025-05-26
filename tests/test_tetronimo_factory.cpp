#include "tetris/TetronimoFactory.h"
#include "engine/Texture.h"
#include <gtest/gtest.h>
#include <memory>
#include <unordered_map>

class TetronimoFactoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        textures[BLOCK_TEXTURE_RED] = std::make_unique<Texture>();
        textures[BLOCK_TEXTURE_BLUE] = std::make_unique<Texture>();
        textures[BLOCK_TEXTURE_YELLOW] = std::make_unique<Texture>();
        textures[BLOCK_TEXTURE_GREEN] = std::make_unique<Texture>();
        textures[BLOCK_TEXTURE_PURPLE] = std::make_unique<Texture>();
        textures[BLOCK_TEXTURE_ORANGE] = std::make_unique<Texture>();
        textures[BLOCK_TEXTURE_NAVY] = std::make_unique<Texture>();
        
        factory = std::make_unique<TetronimoFactory>(textures);
    }
    
    std::unordered_map<std::string_view, std::unique_ptr<Texture>> textures;
    std::unique_ptr<TetronimoFactory> factory;
};

TEST_F(TetronimoFactoryTest, CreatesTetromino)
{
    Grid tetromino = factory->getNextTetronimo();
    
    // Should create something
    EXPECT_GT(tetromino.getHeight(), 0);
    EXPECT_GT(tetromino.getWidth(), 0);
    
    // Should start at correct position
    EXPECT_EQ(tetromino.getPosX(), TETRONIMO_START_X);
    EXPECT_EQ(tetromino.getPosY(), TETRONIMO_START_Y);
}

TEST_F(TetronimoFactoryTest, TetrominoHasFourBlocks)
{
    Grid tetromino = factory->getNextTetronimo();
    
    int count = 0;
    for (size_t y = 0; y < tetromino.getHeight(); ++y) {
        for (size_t x = 0; x < tetromino.getWidth(); ++x) {
            if (tetromino.getBlock(x, y).exists()) {
                count++;
            }
        }
    }
    
    EXPECT_EQ(count, 4);
}

TEST_F(TetronimoFactoryTest, TetrominoHasTexture)
{
    Grid tetromino = factory->getNextTetronimo();
    
    bool foundBlockWithTexture = false;
    for (size_t y = 0; y < tetromino.getHeight(); ++y) {
        for (size_t x = 0; x < tetromino.getWidth(); ++x) {
            if (tetromino.getBlock(x, y).exists()) {
                EXPECT_NE(tetromino.getBlock(x, y).getTexture(), nullptr);
                foundBlockWithTexture = true;
            }
        }
    }
    
    EXPECT_TRUE(foundBlockWithTexture);
}

TEST_F(TetronimoFactoryTest, ConsecutiveCallsWork)
{
    // Just make sure we can call it multiple times without crashing
    for (int i = 0; i < 10; ++i) {
        Grid tetromino = factory->getNextTetronimo();
        EXPECT_EQ(tetromino.getPosX(), TETRONIMO_START_X);
        EXPECT_EQ(tetromino.getPosY(), TETRONIMO_START_Y);
    }
}