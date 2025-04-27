cmake_minimum_required(VERSION 3.10)
project(Tetris VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_HOST_WIN32 )
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # Static linking
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(CMAKE_HOST_WIN32 )

# Manually set SDL2 include and library paths
if(CMAKE_HOST_WIN32 )
    set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}\\..\\SDL2-2.32.0\\include")
    set(SDL2_LIBRARY "${CMAKE_SOURCE_DIR}\\..\\SDL2-2.32.0\\lib\\x64\\SDL2.lib")
    set(SDL2_LIBRARY_DLL "${CMAKE_SOURCE_DIR}\\..\\SDL2-2.32.0\\lib\\x64\\SDL2.dll")

    set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}\\..\\SDL2_image-2.8.8\\include")
    set(SDL2_IMAGE_LIBRARY "${CMAKE_SOURCE_DIR}\\..\\SDL2_image-2.8.8\\lib\\x64\\SDL2_image.lib")
    set(SDL2_IMAGE_LIBRARY_DLL "${CMAKE_SOURCE_DIR}\\..\\SDL2_image-2.8.8\\lib\\x64\\SDL2_image.dll")

    set(SDL2_TTF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}\\..\\SDL2_ttf-2.24.0\\include")
    set(SDL2_TTF_LIBRARY "${CMAKE_SOURCE_DIR}\\..\\SDL2_ttf-2.24.0\\lib\\x64\\SDL2_ttf.lib")
    set(SDL2_TTF_LIBRARY_DLL "${CMAKE_SOURCE_DIR}\\..\\SDL2_ttf-2.24.0\\lib\\x64\\SDL2_ttf.dll")
else()
    set(SDL2_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2.dylib")

    set(SDL2_IMAGE_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_IMAGE_LIBRARY "/opt/homebrew/lib/libSDL2_image.dylib")

    set(SDL2_TTF_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_TTF_LIBRARY "/opt/homebrew/lib/libSDL2_ttf.dylib")
endif(CMAKE_HOST_WIN32 )

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

if (WIN32)
    add_custom_command(
        TARGET tetris_game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:tetris_game>"
        VERBATIM
    )
    add_custom_command(
        TARGET tetris_game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:tetris_game>"
        VERBATIM
    )
    add_custom_command(
        TARGET tetris_game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:tetris_game>"
        VERBATIM
    )
endif()
