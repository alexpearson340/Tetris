#include "tetris/TetronimoFactory.h"

TetronimoFactory::TetronimoFactory(std::unordered_map<std::string_view, Texture>& textures)
    : mTextures { textures }
{
    setup();
}

void TetronimoFactory::setup()
{
    // Seed the random number generator
    std::random_device rd;
    mGen = std::mt19937(rd());
    mDis = std::uniform_int_distribution<>(0, 6);
}

Grid TetronimoFactory::getNextTetronimo()
{
    int randomNumber { mDis(mGen) };
    Grid grid { mTetronimoStartX, mTetronimoStartY, 0, 0 };
    switch (randomNumber)
    {
    case 0:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 0, &mTextures.at(BLOCK_TEXTURE_RED));
        grid.createBlock(1, 0, &mTextures.at(BLOCK_TEXTURE_RED));
        grid.createBlock(1, 1, &mTextures.at(BLOCK_TEXTURE_RED));
        grid.createBlock(2, 1, &mTextures.at(BLOCK_TEXTURE_RED));
        break;

    case 1:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 4, 4 };
        grid.createBlock(1, 0, &mTextures.at(BLOCK_TEXTURE_BLUE));
        grid.createBlock(1, 1, &mTextures.at(BLOCK_TEXTURE_BLUE));
        grid.createBlock(1, 2, &mTextures.at(BLOCK_TEXTURE_BLUE));
        grid.createBlock(1, 3, &mTextures.at(BLOCK_TEXTURE_BLUE));
        break;

    case 2:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 2, 2 };
        grid.createBlock(0, 0, &mTextures.at(BLOCK_TEXTURE_YELLOW));
        grid.createBlock(1, 0, &mTextures.at(BLOCK_TEXTURE_YELLOW));
        grid.createBlock(0, 1, &mTextures.at(BLOCK_TEXTURE_YELLOW));
        grid.createBlock(1, 1, &mTextures.at(BLOCK_TEXTURE_YELLOW));
        break;

    case 3:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(1, 0, &mTextures.at(BLOCK_TEXTURE_GREEN));
        grid.createBlock(2, 0, &mTextures.at(BLOCK_TEXTURE_GREEN));
        grid.createBlock(0, 1, &mTextures.at(BLOCK_TEXTURE_GREEN));
        grid.createBlock(1, 1, &mTextures.at(BLOCK_TEXTURE_GREEN));
        break;

    case 4:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(1, 0, &mTextures.at(BLOCK_TEXTURE_PURPLE));
        grid.createBlock(0, 1, &mTextures.at(BLOCK_TEXTURE_PURPLE));
        grid.createBlock(1, 1, &mTextures.at(BLOCK_TEXTURE_PURPLE));
        grid.createBlock(2, 1, &mTextures.at(BLOCK_TEXTURE_PURPLE));
        break;

    case 5:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(2, 0, &mTextures.at(BLOCK_TEXTURE_ORANGE));
        grid.createBlock(0, 1, &mTextures.at(BLOCK_TEXTURE_ORANGE));
        grid.createBlock(1, 1, &mTextures.at(BLOCK_TEXTURE_ORANGE));
        grid.createBlock(2, 1, &mTextures.at(BLOCK_TEXTURE_ORANGE));
        break;

    case 6:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 0, &mTextures.at(BLOCK_TEXTURE_NAVY));
        grid.createBlock(0, 1, &mTextures.at(BLOCK_TEXTURE_NAVY));
        grid.createBlock(1, 1, &mTextures.at(BLOCK_TEXTURE_NAVY));
        grid.createBlock(2, 1, &mTextures.at(BLOCK_TEXTURE_NAVY));
        break;

    default:
        Grid grid {mTetronimoStartX, mTetronimoStartY, 0, 0}; // Default initialization
        break;
    }

    return grid;
}