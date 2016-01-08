#include <boost/network/include/http/server.hpp>
#include <boost/thread.hpp>

namespace http = boost::network::http;
namespace util = boost::network::utils;

struct dummy_async_handler;
typedef http::server<dummy_async_handler> async_server;

struct dummy_async_handler {
  void operator()(async_server::request const&,
                  async_server::connection_ptr) {
    // Really, this is just for testing purposes
  }
};

// In this batch of tests we ensure that calling run and stop on an
// async_server, in any sequence, is thread safe.

int main(int, char*[]) {
  dummy_async_handler async_handler;

#define ASYNC_SERVER_TEST_CONFIG \
  options.address("127.0.0.1").port("8007").reuse_address(true)

#define ASYNC_SERVER_SLEEP_TIME boost::posix_time::milliseconds(100)

  // stop from main thread
  {
    BOOST_NETWORK_MESSAGE("TEST: stop without running");
    async_server::options options(async_handler);
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);
    server_instance.stop();
  }

  // run-stop from main thread
  {
    BOOST_NETWORK_MESSAGE("TEST: stop from main thread");
    async_server::options options(async_handler);
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(
        boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    server_instance.stop();
    running_thread.join();
  }

  // run-stop from another thread
  {
    BOOST_NETWORK_MESSAGE("TEST: stop from another thread");
    async_server::options options(async_handler);
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(
        boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(
        boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    running_thread.join();
  }

  // run-stop-run-stop from another thread
  {
    BOOST_NETWORK_MESSAGE("TEST: run-stop-run-stop from another thread");
    async_server::options options(async_handler);
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(
        boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(
        boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_running_thread(
        boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_stopping_thread(
        boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    running_thread.join();
    second_stopping_thread.join();
    second_running_thread.join();
  }

  // run-run-stop from another thread
  {
    BOOST_NETWORK_MESSAGE("TEST: run-run-stop from another thread");
    async_server::options options(async_handler);
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(
        boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_running_thread(
        boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(
        boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    running_thread.join();
    second_running_thread.join();
  }

  // run-stop-stop from another thread
  {
    BOOST_NETWORK_MESSAGE("TEST: run-stop-stop from another thread");
    async_server::options options(async_handler);
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(
        boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(
        boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_stopping_thread(
        boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    second_stopping_thread.join();
    running_thread.join();
  }

#undef ASYNC_SERVER_TEST_CONFIG

  return 0;
}
