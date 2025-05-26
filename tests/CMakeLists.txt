if(CMAKE_HOST_WIN32 )
  option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" ON)
endif(CMAKE_HOST_WIN32 )

add_executable(
  tetris_tests
  test_main.cpp
  test_block.cpp
  test_grid.cpp
  test_tetronimo_factory.cpp
  test_collision_handler.cpp
)

target_link_libraries(
  tetris_tests
  GTest::gtest_main
  tetris_lib
  engine_lib
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${SDL2_TTF_LIBRARY}
)

target_include_directories(tetris_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(WIN32)
  add_custom_command(
    TARGET tetris_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:GTest::gtest> $<TARGET_FILE_DIR:tetris_tests>
    COMMENT "Copying gtest DLL to output directory"
  )
endif()

include(GoogleTest)
gtest_discover_tests(tetris_tests)