#include "gtest/gtest.h"

#include <thread>
#include <shared_mutex>
#include <barrier>
#include <future>

#include <map>


struct ThreadSafeConfig {
  std::map<std::string, int> settings_;
  mutable std::shared_mutex rw_;

  void set(const std::string& name, int value) {
    std::unique_lock<std::shared_mutex> lk(this->rw_);
    this->settings_.insert_or_assign(name, value);
  }

  int get(const std::string& name) {
    std::shared_lock<std::shared_mutex> lk(this->rw_);
    return this->settings_.at(name);
  }
};

TEST(SyncTest, test_shared_mutex) {
  
  ThreadSafeConfig config;
  config.set("value", 3);
  
  std::thread t = std::thread([&]() {
    config.set("value", 4);
    std::cout << "value: " << config.get("value") << "\n";
  });

  std::cout << "value: " << config.get("value") << "\n";
  t.join();
}

TEST(SyncTest, test_barrier) {
  std::barrier b(2, [] { std::cout << "Green flag go!\n"; });

  std::thread tB = std::thread([&]() {
    std::cout << "B is setting up\n";
    b.arrive_and_wait();
    std::cout << "B is running\n";
  });

  std::cout << "A is setting up\n";
  b.arrive_and_wait();
  std::cout << "A is running\n";

  tB.join();
}

TEST(SyncTest, future_test) {
  auto f1 = std::async([]() -> int {
    std::cout << "Hello from thread A \n";
    return 2;
  });

  auto f2 = std::async([]() -> int {
    std::cout << "Hello from thread B \n";
    return 3;
  });

  EXPECT_EQ(5, f1.get() + f2.get());
}