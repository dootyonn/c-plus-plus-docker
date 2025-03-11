#include <gtest/gtest.h>

TEST(lambda_test, lambda_that_keep_state) {
  auto fib = [i = 0, j = 1] () mutable {
    i = std::exchange(j, i + j);
    return i;
  };

  EXPECT_EQ(fib(), 1);
  EXPECT_EQ(fib(), 1);
  EXPECT_EQ(fib(), 2);
  EXPECT_EQ(fib(), 3);
  EXPECT_EQ(fib(), 5);
}
