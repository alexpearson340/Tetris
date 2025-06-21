#ifndef TETRIS_H
#define TETRIS_H

#include "tetris/CollisionHandler.h"
#include "engine/BaseEngine.h"
#include "tetris/Grid.h"
#include "tetris/TetronimoFactory.h"
#include <sstream>

class TetrisGameEngine : public BaseEngine
{
public:
    TetrisGameEngine(); 

private:
    bool loadMedia() override;
    bool create() override;
    bool update() override;
    bool render() override;

    // Updates the information bar texture text
    void updateInformationBar();

    Grid mCurrentTetronimo;
    Grid mGameBoard;
    TetronimoFactory mFactory;
    CollisionHandler mCollisionHandler;

    // A text texture displaying FPS, score, etc
    std::unique_ptr<Texture> mInfoBar;
    std::stringstream mInfoText;
};

#endif