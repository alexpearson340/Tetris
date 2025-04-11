#ifndef TETRONIMOFACTORY_H
#define TETRONIMOFACTORY_H

#include "Grid.h"
#include <random>

// Provides a random Grid representing a Tetronimo on demand
class TetronimoFactory
{
public:
    TetronimoFactory(std::vector<Texture>);

    Grid getNextTetronimo();

private:
    void setup();
    std::mt19937 mGen; // Mersenne Twister generator
    std::uniform_int_distribution<> mDis; // Uniform distribution
    const int mTetronimoStartX { TETRONIMO_START_X };
    const int mTetronimoStartY { TETRONIMO_START_Y };
    std::vector<Texture> mTextures;
};

#endif