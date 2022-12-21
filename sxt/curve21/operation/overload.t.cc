#include "sxt/curve21/operation/overload.h"

#include "sxt/base/test/unit_test.h"
#include "sxt/curve21/type/element_p3.h"
#include "sxt/scalar25/type/literal.h"

using namespace sxt;
using namespace sxt::c21t;
using sxt::s25t::operator""_s25;

TEST_CASE("we can use operators on curve21 elements") {
  c21t::element_p3 e1{
      .X{3990542415680775, 3398198340507945, 4322667446711068, 2814063955482877, 2839572215813860},
      .Y{1801439850948184, 1351079888211148, 450359962737049, 900719925474099, 1801439850948198},
      .Z{1, 0, 0, 0, 0},
      .T{1841354044333475, 16398895984059, 755974180946558, 900171276175154, 1821297809914039},
  };

  SECTION("we do basic operations") {
    REQUIRE(0x2_s25 * e1 == e1 + e1);
    REQUIRE(e1 + (e1 - e1) == e1);
    REQUIRE(e1 + (e1 + -e1) == e1);
    REQUIRE(-(-e1) == e1);
  }

  SECTION("we can use +=") {
    auto e1p = e1;
    e1p += e1;
    REQUIRE(e1p == 0x2_s25 * e1);
  }

  SECTION("we can use -=") {
    auto e1p = e1;
    e1p -= e1;
    REQUIRE(e1p + e1 == e1);
  }
}
