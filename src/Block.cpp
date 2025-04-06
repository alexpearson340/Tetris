#include "tetris/Block.h"

Block::Block()
    : mPosX(0)
    , mPosY(0)
    , mTexture(nullptr)
{
}

Block::Block(int x, int y, Texture* texture)
    : mPosX(x)
    , mPosY(y)
    , mTexture(texture)
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
    // Show the block
    mTexture->render(mPosX, mPosY);
}

Texture* Block::getTexture()
{
    return mTexture;
}

void Block::setTexture(Texture* texture)
{
    mTexture = texture;
}

bool Block::exists()
{
    return mTexture != nullptr;
}