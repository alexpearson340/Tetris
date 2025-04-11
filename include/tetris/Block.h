#ifndef BLOCK_H
#define BLOCK_H

#include "engine/Texture.h"

// The Blocks that will move around on the screen
class Block
{
public:
    Block();

    Block(int, int, Texture*);

    void move(int, int);

    void moveTo(int, int);

    int getPosX();

    int getPosY();

    void render();

    // Having virtual blocks to fill unoccupied Grid squares is easier
    // than the handling required around std::optional<Block> in the Grid
    bool exists();

    Texture* getTexture();

    void setTexture(Texture*);

private:
    int mPosX, mPosY;

    Texture* mTexture;
};

#endif // BLOCK_H