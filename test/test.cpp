#include <gtest/gtest.h>

//#include <vm_pool.H>

//#include "mocks/console_mock.cpp"
#include "test_utils.cpp"
#include "buddy_vm_test.cpp"
#include "bitmap_test.cpp"
#include "memmap_test.cpp"


void Console::putui(const unsigned int i){std::cout << i << std::endl;}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
