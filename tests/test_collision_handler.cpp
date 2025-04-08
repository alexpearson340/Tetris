#include "tetris/CollisionHandler.h"
#include "tetris/Texture.h"
#include <gtest/gtest.h>

class CollisionHandlerTest : public ::testing::Test
{
protected:
    // Set up objects that will be used in multiple tests
    void SetUp() override
    {
        // Create the collision handler with initial time
        currentTime = 0;
        mHandler = CollisionHandler(&whiteTexture, &blackTexture, currentTime);

        // An empty game board
        mGameBoard = Grid(0, 0, N_ROWS, N_COLS);
        mGameBoard.updatePositions();
    }

    Grid getTetronimo(int startPosX, int startPosY)
    {
        Grid tetronimo { startPosX, startPosY, 2, 2 };
        tetronimo.createBlock(0, 0, nullptr);
        tetronimo.createBlock(0, 1, nullptr);
        tetronimo.createBlock(1, 0, nullptr);
        tetronimo.createBlock(1, 1, nullptr);
        return tetronimo;
    }

    Texture whiteTexture { nullptr };
    Texture blackTexture { nullptr };
    CollisionHandler mHandler { nullptr, nullptr, 0 };
    Grid mGameBoard { 0, 0, 0, 0 };
    uint32_t currentTime;
};

TEST_F(CollisionHandlerTest, InitialStateIsKeepPlaying)
{
    EXPECT_TRUE(mHandler.keepPlaying());
}

TEST_F(CollisionHandlerTest, BlockMovement_VerticalOnly)
{
    Grid tetronimo { getTetronimo(160, 0) };
    EXPECT_FALSE(mHandler.handle(tetronimo, mGameBoard, 0));
    EXPECT_TRUE(mHandler.keepPlaying());

    EXPECT_EQ(tetronimo.getBlock(0, 0).getPosX(), 160);
    EXPECT_EQ(tetronimo.getBlock(0, 0).getPosY(), 1);
    EXPECT_EQ(tetronimo.getBlock(0, 1).getPosX(), 200);
    EXPECT_EQ(tetronimo.getBlock(0, 1).getPosY(), 1);
    EXPECT_EQ(tetronimo.getBlock(1, 0).getPosX(), 160);
    EXPECT_EQ(tetronimo.getBlock(1, 0).getPosY(), 41);
    EXPECT_EQ(tetronimo.getBlock(1, 1).getPosX(), 200);
    EXPECT_EQ(tetronimo.getBlock(1, 1).getPosY(), 41);
}

TEST_F(CollisionHandlerTest, BlockMovement_HorizontalLeft)
{
    Grid tetronimo { getTetronimo(160, 0) };
    tetronimo.setVelX(-40);

    EXPECT_FALSE(mHandler.handle(tetronimo, mGameBoard, 50));
    EXPECT_TRUE(mHandler.keepPlaying());

    EXPECT_EQ(tetronimo.getBlock(0, 0).getPosX(), 120);
    EXPECT_EQ(tetronimo.getBlock(0, 0).getPosY(), 1);
    EXPECT_EQ(tetronimo.getBlock(0, 1).getPosX(), 160);
    EXPECT_EQ(tetronimo.getBlock(0, 1).getPosY(), 1);
    EXPECT_EQ(tetronimo.getBlock(1, 0).getPosX(), 120);
    EXPECT_EQ(tetronimo.getBlock(1, 0).getPosY(), 41);
    EXPECT_EQ(tetronimo.getBlock(1, 1).getPosX(), 160);
    EXPECT_EQ(tetronimo.getBlock(1, 1).getPosY(), 41);
}

TEST_F(CollisionHandlerTest, BlockMovement_HorizontalRight)
{
    Grid tetronimo { getTetronimo(160, 0) };
    tetronimo.setVelX(40);

    EXPECT_FALSE(mHandler.handle(tetronimo, mGameBoard, 50));
    EXPECT_TRUE(mHandler.keepPlaying());

    EXPECT_EQ(tetronimo.getBlock(0, 0).getPosX(), 200);
    EXPECT_EQ(tetronimo.getBlock(0, 0).getPosY(), 1);
    EXPECT_EQ(tetronimo.getBlock(0, 1).getPosX(), 240);
    EXPECT_EQ(tetronimo.getBlock(0, 1).getPosY(), 1);
    EXPECT_EQ(tetronimo.getBlock(1, 0).getPosX(), 200);
    EXPECT_EQ(tetronimo.getBlock(1, 0).getPosY(), 41);
    EXPECT_EQ(tetronimo.getBlock(1, 1).getPosX(), 240);
    EXPECT_EQ(tetronimo.getBlock(1, 1).getPosY(), 41);
}