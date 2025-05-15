#ifndef TETRONIMOFACTORY_H
#define TETRONIMOFACTORY_H

#include "tetris/Grid.h"
#include <memory>
#include <random>
#include <unordered_map>

// Provides a random Grid representing a Tetronimo on demand
class TetronimoFactory
{
public:
    TetronimoFactory(std::unordered_map<std::string_view, std::unique_ptr<Texture>>&);

    Grid getNextTetronimo();

private:
    void setup();
    std::mt19937 mGen; // Mersenne Twister generator
    std::uniform_int_distribution<> mDis; // Uniform distribution
    const int mTetronimoStartX { TETRONIMO_START_X };
    const int mTetronimoStartY { TETRONIMO_START_Y };
    std::unordered_map<std::string_view, std::unique_ptr<Texture>>& mTextures;
};

#endif