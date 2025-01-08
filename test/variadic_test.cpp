#include <gtest/gtest.h>

#include <type_traits>

template<typename ... Args>
constexpr auto Sum(Args... args) {
  return (... + args);
}

TEST(VariadicTest, t1) {
  constexpr auto mySum = Sum(1, 2, 3, 4);
  std::cout << "mySum: " << mySum << std::endl;
  static_assert(mySum == 10);
}

template<typename T>
constexpr auto Half(T x) {
  if constexpr (std::is_integral_v<T>) {
    return x / T(2) + (x % T(2));
  }

  return x / T(2);
}

constexpr auto Half2(std::integral auto x) {
  return x / 2 + (x % 2);
}

constexpr auto Half2(std::floating_point auto x) {
  return x / 2;
}

template<typename T>
concept CanHalf = requires(T t) {
  { t / T(2) } -> std::same_as<T>;
};

template <CanHalf T>
requires std::integral<T>
constexpr auto Half3(T x) {
  return x / T(2) + (x % T(2));
}

template <CanHalf T>
constexpr auto Half3(T x) {
  return x / T(2);
}

TEST(VariadicTest, t2) {
  std::cout << "Half(5): " << Half(5) << std::endl;
  std::cout << "Half(217ull): " << Half(217ull) << std::endl;
  std::cout << "Half(2.5f): " << Half(2.5f) << std::endl;

  std::cout << "Half(5): " << Half2(5) << std::endl;
  std::cout << "Half(217ull): " << Half2(217ull) << std::endl;
  std::cout << "Half(2.5f): " << Half2(2.5f) << std::endl;

  std::cout << "Half(5): " << Half3(5) << std::endl;
  std::cout << "Half(217ull): " << Half3(217ull) << std::endl;
  std::cout << "Half(2.5f): " << Half3(2.5f) << std::endl;
}

