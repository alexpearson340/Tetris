#include "tetris/TetronimoFactory.h"

TetronimoFactory::TetronimoFactory(std::unordered_map<std::string_view, std::unique_ptr<Texture>>& textures)
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
        grid.createBlock(0, 0, mTextures.at(BLOCK_TEXTURE_RED).get());
        grid.createBlock(1, 0, mTextures.at(BLOCK_TEXTURE_RED).get());
        grid.createBlock(1, 1, mTextures.at(BLOCK_TEXTURE_RED).get());
        grid.createBlock(2, 1, mTextures.at(BLOCK_TEXTURE_RED).get());
        break;

    case 1:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 4, 4 };
        grid.createBlock(1, 0, mTextures.at(BLOCK_TEXTURE_BLUE).get());
        grid.createBlock(1, 1, mTextures.at(BLOCK_TEXTURE_BLUE).get());
        grid.createBlock(1, 2, mTextures.at(BLOCK_TEXTURE_BLUE).get());
        grid.createBlock(1, 3, mTextures.at(BLOCK_TEXTURE_BLUE).get());
        break;

    case 2:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 2, 2 };
        grid.createBlock(0, 0, mTextures.at(BLOCK_TEXTURE_YELLOW).get());
        grid.createBlock(1, 0, mTextures.at(BLOCK_TEXTURE_YELLOW).get());
        grid.createBlock(0, 1, mTextures.at(BLOCK_TEXTURE_YELLOW).get());
        grid.createBlock(1, 1, mTextures.at(BLOCK_TEXTURE_YELLOW).get());
        break;

    case 3:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(1, 0, mTextures.at(BLOCK_TEXTURE_GREEN).get());
        grid.createBlock(2, 0, mTextures.at(BLOCK_TEXTURE_GREEN).get());
        grid.createBlock(0, 1, mTextures.at(BLOCK_TEXTURE_GREEN).get());
        grid.createBlock(1, 1, mTextures.at(BLOCK_TEXTURE_GREEN).get());
        break;

    case 4:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(1, 0, mTextures.at(BLOCK_TEXTURE_PURPLE).get());
        grid.createBlock(0, 1, mTextures.at(BLOCK_TEXTURE_PURPLE).get());
        grid.createBlock(1, 1, mTextures.at(BLOCK_TEXTURE_PURPLE).get());
        grid.createBlock(2, 1, mTextures.at(BLOCK_TEXTURE_PURPLE).get());
        break;

    case 5:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(2, 0, mTextures.at(BLOCK_TEXTURE_ORANGE).get());
        grid.createBlock(0, 1, mTextures.at(BLOCK_TEXTURE_ORANGE).get());
        grid.createBlock(1, 1, mTextures.at(BLOCK_TEXTURE_ORANGE).get());
        grid.createBlock(2, 1, mTextures.at(BLOCK_TEXTURE_ORANGE).get());
        break;

    case 6:
        grid = Grid { mTetronimoStartX, mTetronimoStartY, 3, 3 };
        grid.createBlock(0, 0, mTextures.at(BLOCK_TEXTURE_NAVY).get());
        grid.createBlock(0, 1, mTextures.at(BLOCK_TEXTURE_NAVY).get());
        grid.createBlock(1, 1, mTextures.at(BLOCK_TEXTURE_NAVY).get());
        grid.createBlock(2, 1, mTextures.at(BLOCK_TEXTURE_NAVY).get());
        break;

    default:
        Grid grid {mTetronimoStartX, mTetronimoStartY, 0, 0}; // Default initialization
        break;
    }

    return grid;
}