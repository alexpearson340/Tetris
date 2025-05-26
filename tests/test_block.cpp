#include "tetris/Block.h"
#include "engine/Texture.h"
#include <gtest/gtest.h>
#include <memory>

// Test fixture for Block tests
class BlockTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create mock textures - nullptr is fine for most tests
        // as we're not actually rendering
        mockTexture = std::make_unique<Texture>();
    }

    std::unique_ptr<Texture> mockTexture;
};

// Test default constructor
TEST_F(BlockTest, DefaultConstructor)
{
    Block block;
    
    EXPECT_EQ(block.getPosX(), 0);
    EXPECT_EQ(block.getPosY(), 0);
    EXPECT_EQ(block.getTexture(), nullptr);
    EXPECT_FALSE(block.exists());
}

// Test parameterized constructor
TEST_F(BlockTest, ParameterizedConstructor)
{
    const int expectedX = 100;
    const int expectedY = 200;
    
    Block block(expectedX, expectedY, mockTexture.get());
    
    EXPECT_EQ(block.getPosX(), expectedX);
    EXPECT_EQ(block.getPosY(), expectedY);
    EXPECT_EQ(block.getTexture(), mockTexture.get());
    EXPECT_TRUE(block.exists());
}

// Test move method with positive values
TEST_F(BlockTest, MovePositiveValues)
{
    Block block(50, 100, mockTexture.get());
    
    block.move(10, 20);
    
    EXPECT_EQ(block.getPosX(), 60);
    EXPECT_EQ(block.getPosY(), 120);
}

// Test move method with negative values
TEST_F(BlockTest, MoveNegativeValues)
{
    Block block(50, 100, mockTexture.get());
    
    block.move(-30, -40);
    
    EXPECT_EQ(block.getPosX(), 20);
    EXPECT_EQ(block.getPosY(), 60);
}

// Test move method with zero values
TEST_F(BlockTest, MoveZeroValues)
{
    Block block(50, 100, mockTexture.get());
    
    block.move(0, 0);
    
    EXPECT_EQ(block.getPosX(), 50);
    EXPECT_EQ(block.getPosY(), 100);
}

// Test cumulative moves
TEST_F(BlockTest, MultipleMoves)
{
    Block block(0, 0, mockTexture.get());
    
    block.move(10, 20);
    block.move(5, -10);
    block.move(-20, 15);
    
    EXPECT_EQ(block.getPosX(), -5);
    EXPECT_EQ(block.getPosY(), 25);
}

// Test moveTo method
TEST_F(BlockTest, MoveTo)
{
    Block block(100, 200, mockTexture.get());
    
    block.moveTo(300, 400);
    
    EXPECT_EQ(block.getPosX(), 300);
    EXPECT_EQ(block.getPosY(), 400);
}

// Test moveTo with negative coordinates
TEST_F(BlockTest, MoveToNegativeCoordinates)
{
    Block block(100, 200, mockTexture.get());
    
    block.moveTo(-50, -100);
    
    EXPECT_EQ(block.getPosX(), -50);
    EXPECT_EQ(block.getPosY(), -100);
}

// Test moveTo doesn't affect texture
TEST_F(BlockTest, MoveToPreservesTexture)
{
    Block block(0, 0, mockTexture.get());
    Texture* originalTexture = block.getTexture();
    
    block.moveTo(500, 600);
    
    EXPECT_EQ(block.getTexture(), originalTexture);
}

// Test setTexture
TEST_F(BlockTest, SetTexture)
{
    Block block(0, 0, nullptr);
    EXPECT_FALSE(block.exists());
    
    block.setTexture(mockTexture.get());
    
    EXPECT_EQ(block.getTexture(), mockTexture.get());
    EXPECT_TRUE(block.exists());
}

// Test exists() method
TEST_F(BlockTest, ExistsWithTexture)
{
    Block block(0, 0, mockTexture.get());
    EXPECT_TRUE(block.exists());
}

TEST_F(BlockTest, ExistsWithoutTexture)
{
    Block block(0, 0, nullptr);
    EXPECT_FALSE(block.exists());
}

// Test that exists() changes when texture changes
TEST_F(BlockTest, ExistsChangesWithTexture)
{
    Block block;
    EXPECT_FALSE(block.exists());
    
    block.setTexture(mockTexture.get());
    EXPECT_TRUE(block.exists());
    
    block.setTexture(nullptr);
    EXPECT_FALSE(block.exists());
}