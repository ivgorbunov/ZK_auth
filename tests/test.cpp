#include <gtest/gtest.h>

#include "bigint_test_helper.hpp"
#include "bigint_arithm_tests.hpp"
#include "bigint_equalities_tests.hpp"
#include "bigint_types_tests.hpp"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}