#ifndef TETRIS_H
#define TETRIS_H

#include "tetris/CollisionHandler.h"
#include "engine/BaseEngine.h"
#include "tetris/Grid.h"
#include "tetris/TetronimoFactory.h"

class TetrisGameEngine : public BaseEngine
{
public:
    TetrisGameEngine(); 

private:
    bool loadMedia() override;
    bool create() override;
    bool update() override;
    bool render() override;

    Grid mCurrentTetronimo;
    Grid mGameBoard;
    TetronimoFactory mFactory;
    CollisionHandler mCollisionHandler;
};

#endif