#include <gtest/gtest.h>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

#if defined(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
# define use_awaitable \
  boost::asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif


namespace Test {

    constexpr int PORT = 55555;

    awaitable<void> echo(tcp::socket socket) {
        try {
            char data[1024];

            while(true) {
                std::size_t n = co_await socket.async_read_some(
                    boost::asio::buffer(data),
                    use_awaitable
                );

                co_await async_write(socket, boost::asio::buffer(data, n), use_awaitable);
            }
        }
        catch (std::exception& e) {
            std::printf("echo Exception: %s\n", e.what());
        }
    }

    awaitable<void> listener() {
        auto executor = co_await this_coro::executor;
        tcp::acceptor acceptor(executor, { tcp::v4(), PORT });

        while(true) {
            tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
            co_spawn(executor, echo(std::move(socket)), detached);
        }
    }


    TEST(TestAwaitable, t1) {
        // try {
        //     boost::asio::io_context io_context(1);

        //     boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        //     signals.async_wait([&](auto, auto){ io_context.stop(); });

        //     co_spawn(io_context, listener(), detached);

        //     io_context.run();
        // }
        // catch (std::exception& e) {
        //     std::printf("Exception: %s\n", e.what());
        // }
    }

}