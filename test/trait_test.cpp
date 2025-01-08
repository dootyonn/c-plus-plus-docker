#include "gtest/gtest.h"

#include <limits>


template <class T, T v>
struct integral_constant
{
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant<T, v>;

  constexpr operator value_type() const noexcept
  {
    return value;
  }

  constexpr value_type operator()() const noexcept 
  {
    return value;
  }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <class T>
struct is_pointer : public false_type
{
};

template <class T>
struct is_pointer<T *> : true_type
{
};

template <class T>
struct is_floating_point : public false_type
{
};

template <>
struct is_floating_point<double> : public true_type
{
};

template <>
struct is_floating_point<float> : public true_type
{
};

class B
{
};

double float_func() { return 0.0; }

TEST(TraitTest, t1)
{
  static_assert(is_pointer<int *>::value);
  static_assert(!is_pointer<int>::value);
  static_assert(!is_pointer<B>::value);

  static_assert(!is_floating_point<B>::value);
  static_assert(!is_floating_point<int>::value);
  static_assert(is_floating_point<float>::value);
  static_assert(is_floating_point<double>::value);
  static_assert(is_floating_point<decltype(3.14)>::value);
  static_assert(is_floating_point<decltype(float_func())>::value);

  constexpr bool is_float = is_floating_point<decltype(3.14)>{};
  if (is_float) {
    std::cout << "is_float is true " << std::endl;
  }
}

template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> f(T value) { return value; } // replace by concepts

TEST(TraitTest, test_enable_if) {
  EXPECT_EQ(f(2u), 2u);
  EXPECT_EQ(f(-2), -2);
  EXPECT_EQ(f(123ull), 123ull);
}

template<class U>
struct my_rank: std::integral_constant<std::size_t, 0u> 
{};

template <class U>
struct my_rank<U[]> : std::integral_constant<std::size_t, 1u + my_rank<U>::value> 
{};

template <class U, std::size_t N>
struct my_rank<U[N]> : std::integral_constant<std::size_t, 1u + my_rank<U>::value>
{};



TEST(TraitTest, test_rank) {

  {
    static constexpr int A = 1;
    static_assert(my_rank<decltype(A)>::value == 0u);
  }

  {
    static constexpr int A[] = { 1, 2, 3 };
    static_assert(my_rank<decltype(A)>::value == 1u);
  }

  {
    static constexpr int B[][1] = { { 1 }, { 2 }, { 3 } };
    static_assert(my_rank<decltype(B)>::value == 2u);
  }
}
