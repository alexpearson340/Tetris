cmake_minimum_required(VERSION 3.10)
project(Tetris VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Manually set SDL2 include and library paths
set(SDL2_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2.dylib")

set(SDL2_IMAGE_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(SDL2_IMAGE_LIBRARY "/opt/homebrew/lib/libSDL2_image.dylib")

set(SDL2_TTF_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(SDL2_TTF_LIBRARY "/opt/homebrew/lib/libSDL2_ttf.dylib")

# Add include directories
include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
)

# Add executable
add_executable(main 
    main.cpp 
    Tetris.cpp
    Texture.cpp
)

# Set assets directory relative to the source
target_compile_definitions(main PRIVATE ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")

# Link libraries
target_link_libraries(main 
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}
)

# Debug configuration
set(CMAKE_BUILD_TYPE Debug)