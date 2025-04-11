#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL.h>

// global constants
constexpr int BLOCK_SIZE = 40;
constexpr int VERTICAL_VELOCITY = 1;
constexpr int VERTICAL_FAST_VELOCITY = 3 * VERTICAL_VELOCITY;

constexpr int INPUT_INTERVAL_MS = 50;
constexpr int COMPLETED_ROW_FLASH_INTERVAL_MS = 100;
constexpr int N_ROW_FLASHES = 4;

constexpr int N_ROWS = 22;
constexpr int N_COLS = 10;
constexpr int START_LINE = 2 * BLOCK_SIZE; // N_ROWS - 2 is playable
constexpr int SCREEN_WIDTH { BLOCK_SIZE * N_COLS };
constexpr int SCREEN_HEIGHT { (BLOCK_SIZE * N_ROWS) + 24 }; // TODO
constexpr int TETRONIMO_START_X = 120;
constexpr int TETRONIMO_START_Y = 0;

// Different textures for blocks
enum BlockTextures
{
    BLOCK_TEXTURE_RED,
    BLOCK_TEXTURE_BLUE,
    BLOCK_TEXTURE_YELLOW,
    BLOCK_TEXTURE_GREEN,
    BLOCK_TEXTURE_PURPLE,
    BLOCK_TEXTURE_ORANGE,
    BLOCK_TEXTURE_NAVY,
    BLOCK_TEXTURE_GREY,
    BLOCK_TEXTURE_WHITE,
    BLOCK_TEXTURE_BLACK,
};

#endif // CONSTANTS_H