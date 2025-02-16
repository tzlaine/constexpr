#include "support.hpp"
#include <catch2/catch_test_macros.hpp>
#include <constant_wrapper.hpp>
#include <iostream>
#include <sstream>

struct nothing {};

struct hana : std::constant_wrapper<32> {};

#if SUPPORT_ARRAY_VALUES
TEST_CASE("basic string type") {
  constexpr auto hello = std::cw<"hello">;
  REQUIRE(type_of<hello> == type<std::constant_wrapper<"hello">>);
}
#endif

  constexpr auto initial_phase(auto quantity_1, auto quantity_2) {
    return quantity_1 + quantity_2;
  }

  constexpr auto middle_phase(auto tbd) {
    return tbd;
  }

  void final_phase(auto gathered, auto available) {
    if constexpr (gathered == available)
      std::cout << "Profit!\n";
  }

  void impeccable_underground_planning() {
    constexpr auto gathered_quantity = middle_phase(initial_phase(std::cw<42>, std::cw<13>));
    static_assert(gathered_quantity == 55);
    constexpr auto all_available = std::cw<55>;
    final_phase(gathered_quantity, all_available);
  }

  // ill-formed
  // void deeply_flawed_underground_planning() {
  //   constexpr auto gathered_quantity = middle_phase(initial_phase(42, 13));
  //   constexpr auto all_available = 55;
  //   final_phase(gathered_quantity, all_available);
  // }

TEST_CASE("wording example") {
  impeccable_underground_planning();
  // deeply_flawed_underground_planning(); // ill-formed
}

TEST_CASE("unary ops") {
  constexpr auto minus = -std::cw<42>;
  REQUIRE(type_of<minus> == type<std::constant_wrapper<(-42)>>);
}

TEST_CASE("binary ops") {
  constexpr auto sum = std::cw<42> + std::cw<10>;
  REQUIRE(type_of<sum> == type<std::constant_wrapper<52>>);
}

TEST_CASE("construction from std::array") {
  [[maybe_unused]] constexpr auto array = std::cw<std::array<int, 2>{42, 13}>;
  REQUIRE(array[std::cw<1>] == 13);
}

#if SUPPORT_ARRAY_VALUES
TEST_CASE("string array access") {
  constexpr auto a = std::cw<"hello">[std::cw<1>];
  REQUIRE(a() == 'e');
  constexpr auto b = std::cw<'e'>;
  REQUIRE(b() == 'e');
  // constexpr auto b = std::cw<"e">;
  constexpr auto c = (a == b);
  REQUIRE(c() == true);
  REQUIRE(c == true);
}
#endif

TEST_CASE("stream interaction basic") {
  auto ss = std::stringstream{};
  ss << std::cw<42>;
  // h.operator++();
  REQUIRE(ss.str() == "42");
}

#if SUPPORT_ARRAY_VALUES
TEST_CASE("stream interaction string") {
  auto ss = std::stringstream{};
  ss << std::cw<"hello">;
  REQUIRE(ss.str() == "hello");
}

TEST_CASE("array") {
  constexpr int arr[] = {1, 2, 3, 4};
  constexpr auto array = std::cw<arr>;
  REQUIRE(array[std::cw<0>] == std::cw<1>);
  REQUIRE(array[std::cw<1>] == std::cw<2>);
  REQUIRE(array[std::cw<2>] == std::cw<3>);
  REQUIRE(array[std::cw<3>] == std::cw<4>);
}
#endif

TEST_CASE("semi modifying ops") {
  constexpr auto x = std::cw<1>;
  constexpr auto dec = --x;
  constexpr auto inc = ++x;
  REQUIRE(type_of<dec> == type<std::constant_wrapper<0>>);
  REQUIRE(type_of<inc> == type<std::constant_wrapper<2>>);
}

TEST_CASE("semi modifying ops binary") {
  constexpr auto x = std::cw<1>;
  constexpr auto dec = x -= std::cw<2>;
  constexpr auto inc = x += std::cw<2>;
  REQUIRE(type_of<dec> == type<std::constant_wrapper<(-1)>>);
  REQUIRE(type_of<inc> == type<std::constant_wrapper<3>>);
}

TEST_CASE("semi modifying ops assignment") {
  constexpr auto x = std::cw<1>;
  constexpr auto assigned = x = std::cw<2>;
  REQUIRE(type_of<assigned> == type<std::constant_wrapper<(2)>>);
}

template <typename T> constexpr T copy(T v) noexcept { return v; }

TEST_CASE("binary op non constwrapper") {
  constexpr auto x = std::cw<1>;
  constexpr auto dec = x - 2;
  constexpr auto inc = x + 2;
  REQUIRE(dec == -1);
  REQUIRE(inc == 3);
}

#if SUPPORT_ARRAY_VALUES
TEST_CASE("deduce string size")
{
    [[maybe_unused]] constexpr auto x = std::cw<"hello">;
    [[maybe_unused]] const auto test = []<size_t N>(const char(&)[N]) {
        REQUIRE(N == 6);
    };
}
#endif
