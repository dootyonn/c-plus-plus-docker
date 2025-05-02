#include <gtest/gtest.h>

#include <iostream>
#include <array>
#include <memory>
#include <type_traits>
#include <queue>

using byte = unsigned char ;

struct Serializable {
  Serializable(const byte* input, size_t input_size) 
  : data(&input[0], &input[input_size])
  {}

  std::vector<byte> data;
};

template <typename T>
Serializable serialize(T value) {
  static_assert( std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

  const byte* begin = reinterpret_cast<const byte*>(std::addressof(value));
  return Serializable(begin, sizeof(T));
}

template <>
Serializable serialize<std::string_view>(std::string_view value) {
  const byte* begin = reinterpret_cast<const byte*>(value.data());
  return Serializable(begin, value.size() * sizeof(char));
}

template <typename T>
void deserialize(const Serializable& serializable, T& value) {
  static_assert( std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");
  byte* begin_object = reinterpret_cast<byte*>(std::addressof(value)) ;
  std::copy(
    &serializable.data[0],
    &serializable.data[serializable.data.size()],
    begin_object
  );
}

template <>
void deserialize<std::string_view>(
  const Serializable& serializable,
  std::string_view& value
) {

  value = std::string_view(
    reinterpret_cast<const char*>(&serializable.data[0]),
    serializable.data.size()
  );
}

template <int MAX_SIZE = 64>
class Logger {
public:
  Logger() {

  }

  template <typename T>
  void log(const T& value) {
    //std::cout << value;
    std::lock_guard<std::mutex> scope(to_print_mutex);

    // TODO: use factory of message
    //to_print.push(msg);
  }

  template <typename T, typename... Ts>
  void log(const T& value, Ts... args) {
    log(value);
    log(args...);
  }

private:
  std::mutex to_print_mutex;
  std::queue<Serializable> to_print;

};

struct A {
  A() 
  : a(1)
  {}

  int a;
};

TEST(LoggerTest, test_serializable) {
  {
    static constexpr float orig = 43.2f;
    Serializable s = serialize(orig);

    float result;
    deserialize(s, result);

    EXPECT_EQ(result, orig);
  }

  {
    static constexpr int orig = -43;
    Serializable s = serialize(orig);

    int result;
    deserialize(s, result);

    EXPECT_EQ(result, orig);
  }

  {
    static constexpr int orig = -43;
    Serializable s = serialize(orig);

    int result;
    deserialize(s, result);

    EXPECT_EQ(result, orig);
  }

  {
    A orig;
    Serializable s = serialize(orig);

    A result;
    deserialize(s, result);

    EXPECT_EQ(result.a, 1);
  }

  {
    static constexpr char orig[] = "This is a string";
    Serializable s = serialize(orig);

    std::string_view result;
    deserialize(s, result);

    std::cout << "result: " << result << '\n';

    EXPECT_TRUE(strncmp(orig, &result[0], sizeof(orig)) == 0);
  }
}


TEST(LoggerTest, main) {
  Logger logger;

  logger.log(1, 2, "val");
}