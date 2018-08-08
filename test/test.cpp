#include <gtest/gtest.h>

//#include <vm_pool.H>

#include "test_utils.cpp"
#include "test_vmpool.cpp"
//#include "console_test.cpp"


void Console::putui(const unsigned int i){std::cout << i << std::endl;}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
