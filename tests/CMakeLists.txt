add_executable(
  tetris_tests
  test_main.cpp
  test_block.cpp
  test_collision_handler.cpp
)

target_link_libraries(tetris_tests
  engine_lib
  tetris_lib
  GTest::gtest_main
  # for now link in SDL, until we have a better mocking solution
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${SDL2_TTF_LIBRARY}
)

include(GoogleTest)
gtest_discover_tests(tetris_tests)