if(CMAKE_HOST_WIN32 )
  option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" ON)
endif(CMAKE_HOST_WIN32 )

add_executable(
  tetris_tests
  test_main.cpp
  test_block.cpp
)

target_link_libraries(
  tetris_tests
  GTest::gtest_main
)

target_include_directories(tetris_tests PRIVATE ${CMAKE_SOURCE_DIR})

if(WIN32)
  add_custom_command(
    TARGET tetris_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:GTest::gtest> $<TARGET_FILE_DIR:tetris_tests>
    COMMENT "Copying gtest DLL to output directory"
  )
endif()

include(GoogleTest)
gtest_discover_tests(tetris_tests)