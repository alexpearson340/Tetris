add_executable(
  tetris_tests
  test_main.cpp
)

target_link_libraries(
  tetris_tests
  GTest::gtest_main
)

target_include_directories(tetris_tests PRIVATE ${CMAKE_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(tetris_tests)