#include <gtest/gtest.h>

#include <functional>
#include <memory_resource>
#include <list>

constexpr int total_nodes = 20000;

struct A {
    int m1;
    double m2;
    std::string m3;
};

// constexpr const std::size_t CalculatePadding(const std::size_t baseAddress, const std::size_t alignment) {
//     const std::size_t multiplier = (baseAddress / alignment) + 1;
//     const std::size_t alignedAddress = multiplier * alignment;
//     const std::size_t padding = alignedAddress - baseAddress;
//     return padding;
// }

class AllocatorBuffer {
public:
    AllocatorBuffer(size_t buffer_size) 
    : m_used(0)
    {
        m_buffer.resize(buffer_size);
    }

    template<typename T>
    T* allocate(std::size_t count) {
        size_t offset = get_offset<T>();
        size_t alloc_size = count * sizeof(T);

        if (m_used + offset + alloc_size >= m_buffer.size()) {
            //return 0;
            throw std::bad_alloc(); 
        }

        T* result = reinterpret_cast<T*>(m_buffer.data() + m_used + offset);
        m_used += offset + alloc_size;

        return result;
    }

    void reset() {
        m_used = 0;
    }

    void deallocate(void*, std::size_t) { }

private:

    template<typename T>
    inline size_t get_offset() const {
        return alignof(T) - (m_used % alignof(T));
    }

    // std::array<std::byte, TBufferSize> m_buffer;
    alignas(alignof(std::max_align_t)) std::vector<std::byte> m_buffer;
    size_t m_used;
};

template <typename T> 
struct StackAllocator {
    using value_type = T; // Required

    // We need to initialize the first allocator with
    // our buffer.
    StackAllocator(AllocatorBuffer* storage) 
    : m_storage(storage) 
    {}

    StackAllocator(const StackAllocator&) = default;

    // Conversion constructor that passes the buffer along.
    template <typename U>
    StackAllocator(const StackAllocator<U>& other) 
    : m_storage(other.m_storage) 
    {}

    T* allocate(std::size_t n) {
        return m_storage->allocate<T>(n);
    }

    void deallocate(T* p, std::size_t n) {
        m_storage->deallocate(p, n);
    }

    void reset() {
        m_storage->reset();
    }
private:
    // Required for the conversion constructor.
    template <typename U> friend struct StackAllocator;
    // Pointer to the buffer, note that this will increase
    // the size of each container that uses this allocator 
    // by sizeof(StackBuffer*), e.g. 8 bytes on x86-64.
    AllocatorBuffer* m_storage;
};






class Allocator {
protected:
    std::size_t m_totalSize;
    std::size_t m_used;   
    std::size_t m_peak;
public:
    
    Allocator(const std::size_t totalSize) : m_totalSize { totalSize }, m_used { 0 }, m_peak { 0 } { }

    virtual ~Allocator() { m_totalSize = 0; }

    virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) = 0;

    virtual void Free(void* ptr) = 0;

    virtual void Init() = 0;

    friend class Benchmark;
};

auto benchmark(std::function<void()> test_func, int iterations)
{
    const auto start = std::chrono::system_clock::now();
    while (iterations-- > 0)
        test_func();
    const auto stop = std::chrono::system_clock::now();
    const auto secs = std::chrono::duration<double>(stop - start);
    return secs.count();
}

TEST(allocators_test, test_std_allocator_int) {
    const double result = benchmark([]() {
        std::allocator<A> a;

        std::list<int> my_list(a);
        for (int i = 0; i < total_nodes; ++i) {
            my_list.push_back(i);
        }

    }, 1000);

    std::cout << "benchmark: " << result << '\n';
}

TEST(allocators_test, test_StackAllocator_int) {
    AllocatorBuffer sb(1024 * 1024);
    StackAllocator<A> my_allocator(&sb);

    const double result = benchmark([&my_allocator]() {
        std::list<int, StackAllocator<int>> my_list(my_allocator);
        for (int i = 0; i < total_nodes; ++i) {
            my_list.push_back(i);
        }

        my_allocator.reset();

    }, 1000);

    std::cout << "benchmark: " << result << '\n';
}

TEST(allocators_test, test_polymorphic_mem_resource_allocator_int) {

    const double result = benchmark([]() {
        std::array<std::byte, total_nodes * sizeof(int) * 2> buffer;
        std::pmr::monotonic_buffer_resource mbr(buffer.data(), buffer.size());
        std::pmr::polymorphic_allocator<int> pa(&mbr);
        
        std::pmr::list<int> my_list(pa);
        for (int i = 0; i < total_nodes; ++i) {
            my_list.push_back(i);
        }
    }, 1000);

    std::cout << "benchmark: " << result << '\n';
}

TEST(allocators_test, test_std_allocator_structA) {
    std::allocator<A> a;

    const double result = benchmark([&a]() {
        std::list<A> my_list(a);
        for (int i = 0; i < total_nodes; ++i) {
            my_list.push_back(A{});
        }

    }, 1000);

    std::cout << "benchmark: " << result << '\n';
}

TEST(allocators_test, test_StackAllocator_structA) {
    AllocatorBuffer sb(2048 * 1024);
    StackAllocator<A> my_allocator(&sb);

    const double result = benchmark([&my_allocator]() {
        std::list<A, StackAllocator<A>> my_list(my_allocator);
        for (int i = 0; i < total_nodes; ++i) {
            my_list.push_back(A{});
        }

        my_allocator.reset();

    }, 1000);

    std::cout << "benchmark: " << result << '\n';
}

TEST(allocators_test, test_polymorphic_mem_resource_allocator_structA) {

    const double result = benchmark([]() {
        std::array<std::byte, total_nodes * sizeof(A) * 2> buffer;
        std::pmr::monotonic_buffer_resource mbr(buffer.data(), buffer.size());
        std::pmr::polymorphic_allocator<A> pa(&mbr);
        
        std::pmr::list<A> my_list(pa);
        for (int i = 0; i < total_nodes; ++i) {
            my_list.push_back(A{});
        }
    }, 1000);

    std::cout << "benchmark: " << result << '\n';
}