#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Grid.h"
#include "Texture.h"

// Handles the horizontal, rotational and vertical
// collision scenarios, as well as freezing Tetronimos
// when they stop moving, and deleting rows which the
// players had completed
class CollisionHandler
{
public:
    CollisionHandler(SDL_Renderer*, Texture*, Texture*, uint32_t);

    bool handle(Grid&, Grid&, uint32_t);

    bool keepPlaying();

private:
    void handleHorizontal(Grid&, Grid&);

    void handleRotational(Grid&, Grid&);

    bool handleVertical(Grid&, Grid&);

    void freezeTetronimo(Grid&, Grid&);

    void handleCompletedRows(Grid&, Grid&);

    bool checkForCompletedRow(int, Grid&);

    void flashRows(std::vector<int>, Grid&, Texture*);

    bool checkCollisions(Grid&, Grid&);

    bool mKeepPlaying = true;
    Uint32 mPreviousTime;
    Uint32 mCurrentTime;
    Texture* mWhiteFlashTexture;
    Texture* mBlackFlashTexture;

    SDL_Renderer* mRenderer;
};

#endif