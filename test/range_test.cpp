#include <gtest/gtest.h>
#include <ranges>

TEST(TestRanges, t1)
{
  auto const ints = { 0, 1, 2, 3, 4, 5 };
  auto is_even = [](int i) { return i % 2 == 0; };
  auto square = [](int i) { return i * i; };

  for (int i : ints | std::views::filter(is_even) | std::views::transform(square)) {
    std::cout << i << ' ';
  }
  std:: cout << std::endl;

}