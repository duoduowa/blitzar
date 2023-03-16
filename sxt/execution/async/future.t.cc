#include "sxt/execution/async/future.h"

#include <memory>

#include "sxt/base/test/unit_test.h"

using namespace sxt;
using namespace sxt::xena;

TEST_CASE("future manages an asynchronously computed result") {
  SECTION("we can default construct a void future") {
    future<> fut;
    REQUIRE(fut.promise() == nullptr);
    REQUIRE(!fut.ready());
  }

  SECTION("we can default construct a non-void future") {
    future<int> fut;
    REQUIRE(fut.promise() == nullptr);
    REQUIRE(!fut.ready());
  }

  SECTION("we can construct a ready future with a value") {
    auto fut = make_ready_future<std::unique_ptr<int>>(std::make_unique<int>(123));
    REQUIRE(fut.ready());
    REQUIRE(*fut.value() == 123);
  }

  SECTION("we can construct a ready void future") {
    auto fut = make_ready_future();
    REQUIRE(fut.ready());
  }

  SECTION("we can construct a future from a promise") {
    promise<> pr;
    future<> fut{pr};
    REQUIRE(!fut.ready());
    REQUIRE(fut.promise() == &pr);
    pr.make_ready();
    REQUIRE(fut.ready());
  }

  SECTION("we can move-construct a future with no promise") {
    future<> fut1;
    future<> fut2{std::move(fut1)};
    REQUIRE(!fut2.ready());
  }

  SECTION("we can move-construct a future with a promise") {
    promise<> pr;
    future<> fut1{pr};
    future<> fut2{std::move(fut1)};
    REQUIRE(fut1.promise() == nullptr);
    REQUIRE(!fut2.ready());
    REQUIRE(fut2.promise() == &pr);
    pr.make_ready();
    REQUIRE(fut2.ready());
  }

  SECTION("we can move-construct a non-void future with a promise") {
    promise<int> pr;
    future<int> fut1{pr};
    future<int> fut2{std::move(fut1)};
    REQUIRE(fut1.promise() == nullptr);
    REQUIRE(!fut2.ready());
    REQUIRE(fut2.promise() == &pr);

    REQUIRE(&fut2.state() == &pr.state());

    pr.set_value(123);

    REQUIRE(fut2.value() == 123);
  }

  SECTION("a future can be destroyed before the promise is complete") {
    promise<int> pr;
    { future<int> fut{pr}; }
    pr.set_value(123);
  }

  SECTION("a future is valid after its promise has been move constructed") {
    promise<> pr1;
    future<> fut{pr1};
    promise<> pr2{std::move(pr1)};
    REQUIRE(pr1.future() == nullptr);
    REQUIRE(fut.promise() == &pr2);
    pr2.make_ready();
  }

  SECTION("a future is valid after its promise has been move assigned") {
    promise<> pr1;
    future<> fut{pr1};
    promise<> pr2;
    pr2 = std::move(pr1);
    REQUIRE(pr1.future() == nullptr);
    REQUIRE(fut.promise() == &pr2);
    pr2.make_ready();
  }

  SECTION("we can move-assign a future") {
    promise<> pr1;
    future<> fut1{pr1};

    promise<> pr2;
    future<> fut2{pr2};
    fut1 = std::move(fut2);

    REQUIRE(fut1.promise() == &pr2);
    REQUIRE(fut2.promise() == nullptr);

    pr1.make_ready();
    pr2.make_ready();
  }

  SECTION("we can make a continuation from a ready future") {
    auto val = std::make_unique<int>(123);
    auto fut = make_ready_future(std::move(val));
    auto fut_p = fut.then([](std::unique_ptr<int>&& val) noexcept {
      REQUIRE(*val == 123);
      val.reset();
      return 456;
    });
    REQUIRE(fut_p.value() == 456);
  }

  SECTION("we can make a continuation from a future that's not ready") {
    promise<std::unique_ptr<int>> pr;
    future<std::unique_ptr<int>> fut{pr};
    auto fut_p = fut.then([](std::unique_ptr<int>&& val) noexcept {
      REQUIRE(*val == 123);
      val.reset();
      return 456;
    });
    REQUIRE(!fut_p.ready());
    pr.set_value(std::make_unique<int>(123));
    REQUIRE(fut_p.value() == 456);
  }
}
