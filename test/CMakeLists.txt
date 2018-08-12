cmake_minimum_required(VERSION 2.6)
#set (CMAKE_CXX_STANDARD 17)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../src/include ../src/ ./tests ./mocks) 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests test.cpp)
#set_target_properties(runTests PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread )
