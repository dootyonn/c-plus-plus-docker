#include <gtest/gtest.h>

#include <iostream>



TEST(TestRAII, t1)
{
  auto RAII = [
    obj = []() {
      struct S {
        S() { 
          std::cout << "Constructor" << std::endl;
        }
        ~S() {
          std::cout << "Destructor" << std::endl;
        }
      };

      return S{};
    }()
  ](int value) {
    std::cout << "execute: " << value << std::endl;
  };

  RAII(5);
}