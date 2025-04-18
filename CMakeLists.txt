cmake_minimum_required(VERSION 3.10)
project(Tetris VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Manually set SDL2 include and library paths
set(SDL2_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2.dylib")
set(SDL2_IMAGE_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(SDL2_IMAGE_LIBRARY "/opt/homebrew/lib/libSDL2_image.dylib")
set(SDL2_TTF_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(SDL2_TTF_LIBRARY "/opt/homebrew/lib/libSDL2_ttf.dylib")

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

add_library(engine_lib STATIC
    src/engine/GameEngine.cpp
    src/engine/Texture.cpp
)

add_library(tetris_lib STATIC 
    src/tetris/Block.cpp
    src/tetris/CollisionHandler.cpp
    src/tetris/Grid.cpp
    src/tetris/Tetris.cpp
    src/tetris/TetronimoFactory.cpp
)

add_executable(tetris_game 
    src/main.cpp
)

target_link_libraries(tetris_game
    engine_lib
    tetris_lib
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}
)

# Set assets directory relative to the source
target_compile_definitions(engine_lib PRIVATE 
    ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"
)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1  # or a more recent version if available
)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_subdirectory(tests)

set(CMAKE_BUILD_TYPE Debug)