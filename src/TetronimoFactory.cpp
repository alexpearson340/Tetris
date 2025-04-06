#include "tetris/TetronimoFactory.h"


TetronimoFactory::TetronimoFactory(Texture* textures)
    : mTextures { textures }
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
    Grid grid { mTetronimoStartX, mTetronimoStartY, 0, 0 };
    switch (randomNumber)
    {
    case BLOCK_TEXTURE_RED:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 0, &mTextures[BLOCK_TEXTURE_RED]);
        grid.createBlock(0, 1, &mTextures[BLOCK_TEXTURE_RED]);
        grid.createBlock(1, 1, &mTextures[BLOCK_TEXTURE_RED]);
        grid.createBlock(1, 2, &mTextures[BLOCK_TEXTURE_RED]);
        break;

    case BLOCK_TEXTURE_BLUE:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 4, 4 };
        grid.createBlock(0, 1, &mTextures[BLOCK_TEXTURE_BLUE]);
        grid.createBlock(1, 1, &mTextures[BLOCK_TEXTURE_BLUE]);
        grid.createBlock(2, 1, &mTextures[BLOCK_TEXTURE_BLUE]);
        grid.createBlock(3, 1, &mTextures[BLOCK_TEXTURE_BLUE]);
        break;

    case BLOCK_TEXTURE_YELLOW:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 2, 2 };
        grid.createBlock(0, 0, &mTextures[BLOCK_TEXTURE_YELLOW]);
        grid.createBlock(0, 1, &mTextures[BLOCK_TEXTURE_YELLOW]);
        grid.createBlock(1, 0, &mTextures[BLOCK_TEXTURE_YELLOW]);
        grid.createBlock(1, 1, &mTextures[BLOCK_TEXTURE_YELLOW]);
        break;

    case BLOCK_TEXTURE_GREEN:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 1, &mTextures[BLOCK_TEXTURE_GREEN]);
        grid.createBlock(0, 2, &mTextures[BLOCK_TEXTURE_GREEN]);
        grid.createBlock(1, 0, &mTextures[BLOCK_TEXTURE_GREEN]);
        grid.createBlock(1, 1, &mTextures[BLOCK_TEXTURE_GREEN]);
        break;

    case BLOCK_TEXTURE_PURPLE:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 1, &mTextures[BLOCK_TEXTURE_PURPLE]);
        grid.createBlock(1, 0, &mTextures[BLOCK_TEXTURE_PURPLE]);
        grid.createBlock(1, 1, &mTextures[BLOCK_TEXTURE_PURPLE]);
        grid.createBlock(1, 2, &mTextures[BLOCK_TEXTURE_PURPLE]);
        break;

    case BLOCK_TEXTURE_ORANGE:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 2, &mTextures[BLOCK_TEXTURE_ORANGE]);
        grid.createBlock(1, 0, &mTextures[BLOCK_TEXTURE_ORANGE]);
        grid.createBlock(1, 1, &mTextures[BLOCK_TEXTURE_ORANGE]);
        grid.createBlock(1, 2, &mTextures[BLOCK_TEXTURE_ORANGE]);
        break;

    case BLOCK_TEXTURE_NAVY:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 0, &mTextures[BLOCK_TEXTURE_NAVY]);
        grid.createBlock(1, 0, &mTextures[BLOCK_TEXTURE_NAVY]);
        grid.createBlock(1, 1, &mTextures[BLOCK_TEXTURE_NAVY]);
        grid.createBlock(1, 2, &mTextures[BLOCK_TEXTURE_NAVY]);
        break;

    default:
        Grid grid {
            mTetronimoStartX, mTetronimoStartY, 0, 0
        }; // Default initialization
        break;
    }

    return grid;
}