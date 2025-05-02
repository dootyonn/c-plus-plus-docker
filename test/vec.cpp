#include <gtest/gtest.h>

#include <iostream>
#include <cstring>

static constexpr size_t DEFAULT_SIZE = 8;

template<typename T>
class Vec {
public:
  Vec() 
  : _begin{}
  , _end{}
  , _buffer(new T[DEFAULT_SIZE])
  , _alloc_size(DEFAULT_SIZE)
  {}

  ~Vec() {
    delete[] _buffer;
  }

  size_t size() {
    return _end - _begin;
  }

  void push_back(const T& value) {
    size_t local_size = size();
    if (local_size < _alloc_size) {
      resize();
    }

    _buffer[local_size] = value;
    ++_end;
  }

  T& operator[] (size_t index) {
    assert(index < size());
    return _buffer[index];
  }


private:
  void resize() {
    size_t orig_alloc_size = _alloc_size;
    _alloc_size *= 2;

    T* new_buffer = new T[_alloc_size];
    std::memcpy(new_buffer, _buffer, size() * sizeof(T));

    delete[] _buffer;
    _buffer = new_buffer;
  }

  size_t _begin;
  size_t _end;
  T* _buffer;
  size_t _alloc_size;
};


TEST(VecTest, empty) {
  Vec<int> empty;
  EXPECT_EQ(empty.size(), 0);
}

TEST(VecTest, push_back_from_empty) {
  Vec<int> v1;

  v1.push_back(3);

  EXPECT_EQ(v1.size(), 1);
  EXPECT_EQ(v1[0], 3);

  v1[0] = 4;

  EXPECT_EQ(v1.size(), 1);
  EXPECT_EQ(v1[0], 4);

  v1.push_back(5);

  EXPECT_EQ(v1.size(), 2);
  EXPECT_EQ(v1[0], 4);
  EXPECT_EQ(v1[1], 5);
}