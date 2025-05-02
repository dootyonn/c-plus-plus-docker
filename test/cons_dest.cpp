#include <gtest/gtest.h>

#include <iostream>

using namespace std;

struct A {
  A() {
    cout << "Constructor A" << '\n';
  }

  ~A() {
    //cout << "Destructor A" << '\n';
  }

  A(const A&) = delete;
  A& operator=(const A&) = delete;

  A(const A&& a) {
    
  }
};

struct B : public A{
  B() {
    cout << "Constructor B" << '\n';
  }

  ~B() {
    cout << "Destructor B" << '\n';
  }


};

TEST(ConstructorDestructor, ConstructA) {
  A();
}

TEST(ConstructorDestructor, ConstructB) {
  B();
}

A return_temp() {
  return A();
}

TEST(ConstructorDestructor, CanMove) {

  A a1;
  A a2 = std::move(a1);
  A a3 = return_temp();
}