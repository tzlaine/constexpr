#include "support.hpp"
#include <catch2/catch_test_macros.hpp>
#include <constant_wrapper.hpp>
#include <sstream>

template <typename> struct identify;

#if SUPPORT_ARRAY_VALUES
TEST_CASE("deduce string size for template overload") {
  constexpr auto x = std::cw<"hello">;
  const auto test = []<size_t N>(const char(&)[N]) -> unsigned { return N; };
  // identify<decltype(x)::type> z;
  // identify<decltype(x())> y;
  REQUIRE(test(x()) == 6);
}
#endif

TEST_CASE("int minus unsigned") {
  constexpr auto c = std::cw<42> - std::cw<13u>;
  REQUIRE(type_of<c> == type<std::constant_wrapper<29u>>);
  // identify<decltype(c)> y;
  constexpr auto r = 42 - 13u;
  REQUIRE(type_of<r> == type<unsigned>);
  // identify<decltype(r)> z;
}
