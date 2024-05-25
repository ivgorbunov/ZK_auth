#include <gtest/gtest.h>

// bigint tests
#include "bigint_arithm_tests.hpp"
#include "bigint_equalities_tests.hpp"
#include "bigint_test_helper.hpp"
#include "bigint_types_tests.hpp"
// checks the random_bigint function
#include "bigint_random_tests.hpp"
// moduled bigint tests
#include "moduled_bigint_arithm_tests.hpp"
// primality tests
#include "primality_tests.hpp"

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}