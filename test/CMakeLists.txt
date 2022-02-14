find_package(GTest)

# Custom add_my_test
function(add_my_test targetName libraryName fileName)
  add_executable(${targetName} ${fileName})
  target_link_libraries(${targetName} PRIVATE ${libraryName} GTest::GTest
                                              GTest::gtest_main ${ARGN})
  target_include_directories(${targetName} PRIVATE ${CMAKE_SOURCE_DIR}/src)
  gtest_add_tests(
    TARGET ${targetName}
    EXTRA_ARGS "" AUTO
    TEST_SUFFIX .test)
endfunction()

add_my_test(cli_utest cli cli_utest.cpp)
add_my_test(repo_utest repo repo_utest.cpp cli)
add_my_test(object_utest object object_utest.cpp cryptopp-static)
