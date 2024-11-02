#include <boost/hana.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <iostream>
#include <limits>
#include <string_view>

#define EXAMINE_STREAM_INSERTION_ERRORS 0

#if !EXAMINE_STREAM_INSERTION_ERRORS

#include "include/constant_wrapper.hpp"

namespace std {

namespace detail {
// Everything guarded by this macro is exposition only (and not proposed).
#if !defined(__cpp_lib_constexpr_charconv)
using longest_unsigned_type =
    conditional_t<sizeof(size_t) < sizeof(unsigned long long),
                  unsigned long long, size_t>;

template <char C> constexpr auto to_int() {
  if constexpr ('A' <= C && C <= 'F') {
    return boost::hana::llong_c<C - 'A' + 10>;
  } else if constexpr ('a' <= C && C <= 'f') {
    return boost::hana::llong_c<C - 'a' + 10>;
  } else {
    return boost::hana::llong_c<C - '0'>;
  }
}
#endif

struct ic_base_and_offset_result {
  int base;
  int offset;
};

template <size_t Size, char... Chars>
constexpr ic_base_and_offset_result ic_base_and_offset() {
  constexpr char arr[] = {Chars...};
  if constexpr (arr[0] == '0' && 2 < Size) {
    constexpr bool is_hex = arr[1] == 'x' || arr[1] == 'X';
    constexpr bool is_binary = arr[1] == 'b';

    if constexpr (is_hex)
      return {16, 2};
    else if constexpr (is_binary)
      return {2, 2};
    else
      return {8, 1};
  }
  return {10, 0};
}

template <typename TargetType, char... Chars>
constexpr TargetType ic_parse() // exposition only
{
  constexpr auto size = sizeof...(Chars);

  constexpr auto bao = ic_base_and_offset<size, Chars...>();
  constexpr int base = bao.base;
  constexpr int offset = bao.offset;

#if defined(__cpp_lib_constexpr_charconv)
  // This is really here just for documentation purposes right now,
  // because this is what is proposed in the paper.  No implementation has
  // constexpr from_chars() at the time of this writing.
  // const auto f = std::begin(arr) + offset, l = std::end(arr);
  // TargetType x{};
  // constexpr auto result = from_chars(f, l, x, base);
  // return result.ptr == l && result.ec == errc{} ? x : throw logic_error("");
#else
  // Approximate implementation to work around the lack of a constexpr
  // from_chars() at the time of writing.

  using namespace boost::hana::my_literals;

  constexpr auto digits_ = boost::hana::make_tuple(to_int<Chars>()...);
  constexpr auto digits_trimmed =
      boost::hana::drop_front(digits_, boost::hana::llong_c<offset>);

  // initial is (result, mulitplier)
  constexpr auto initial =
      boost::hana::make_tuple(boost::hana::llong_c<0>, boost::hana::llong_c<1>);
  constexpr auto final = boost::hana::reverse_fold(
      digits_trimmed, initial, [base](auto state, auto element_) {
        constexpr auto result = boost::hana::llong_c<state[0_c].value>;
        constexpr auto multiplier = boost::hana::llong_c<state[1_c].value>;
        constexpr auto element = boost::hana::llong_c<element_.value>;
        static_assert(element * multiplier / multiplier == element,
                      "Overflow detected");
        static_assert(result + element * multiplier - element * multiplier ==
                          result,
                      "Overflow detected");
        static_assert(multiplier * base / base == multiplier,
                      "Overflow detected");
        return boost::hana::make_tuple(
            boost::hana::llong_c<result + element * multiplier>,
            boost::hana::llong_c<multiplier * base>);
      });

  constexpr auto retval = final[0_c].value;
  static_assert(
      retval <= std::numeric_limits<TargetType>::max(),
      "The number parsed in an integral_constant literal is out of the "
      "representable range for the integral type being parsed");
  return retval;
#endif
}

template <char... Chars, std::size_t... Is>
constexpr bool contains_dot(index_sequence<Is...>) {
  constexpr char chars[] = {Chars...};
  return ((chars[Is] == '.') || ...);
}
} // namespace detail

namespace my_literals {
template <char... Chars> constexpr auto operator"" _cw() {
  constexpr bool has_dot =
      detail::contains_dot<Chars...>(make_index_sequence<sizeof...(Chars)>());
  if constexpr (has_dot) {
    return 1.0; // TODO
  } else {
#if defined(__cpp_lib_constexpr_charconv)
    // constexpr auto bao = std::detail::ic_base_and_offset<size, Chars...>();
    // constexpr int base = bao.base;
    // constexpr int offset = bao.offset;

    // This is really here just for documentation purposes right now,
    // because this is what is proposed in the paper.  No implementation
    // has constexpr from_chars() at the time of this writing.
    constexpr auto x = [] {
      char chars[] = {Chars...};
      const auto f = std::begin(chars), l = std::end(chars);
      long long x{};
      auto result = std::from_chars(f, l, x);
      if (result.ptr != l || result.ec != errc{})
        throw logic_error("");
      return x;
    }();

    return constant_wrapper<x>{};
#else
    constexpr auto x = detail::ic_parse<long long, Chars...>();
    return constant_wrapper<x>{};
#endif
  }
}
} // namespace my_literals

} // namespace std

// increment and decrement operators intentionally left out
// operator OP= intentionally left out

struct my_nttp {
  template <typename T, typename U> constexpr auto operator()(T t, U u) const {
    return t + u;
  }
};

template <size_t N> struct strlit {
  constexpr strlit(char const (&str)[N]) { std::copy_n(str, N, bytes_); }

  template <size_t M> constexpr bool operator==(strlit<M> rhs) const {
    return std::ranges::equal(bytes_, rhs.bytes_);
  }

  friend std::ostream &operator<<(std::ostream &os, strlit l) {
    assert(!l.bytes_[N - 1] && "bytes_ must be null-terminated");
    return os.write(l.bytes_, N - 1);
  }

  char bytes_[N];
};

// g++-13 ./const.cpp -o const.bin -DDEBUG -std=c++23
// -I/home/tzlaine/boost_1_82_0 && ./const.bin

template <typename T> struct my_complex {
  constexpr my_complex(float r, float i) : re{r}, im{i} {}
  T re, im;
};

#if __clang__
template <typename T> my_complex(T, T) -> my_complex<T>;
#endif

template <typename T> struct X {
  void f(auto c) {}
};

inline constexpr short foo = 2;

template <typename T> void g(X<T> x) {
  x.f(std::cw<1>);
  x.f(std::cw<2>);
#if !__clang__
  x.f(std::cw<3.0>);
  x.f(std::cw<4.f>);
#endif
  x.f(std::cw<foo>);
  x.f(std::cw<my_complex(1.f, 1.f)>);
}

namespace parse {

template <typename L, typename R> struct or_parser;

template <size_t N> struct str_parser {
  constexpr explicit str_parser(strlit<N> s) : str_{s} {}
  template <size_t M> constexpr bool operator()(strlit<M> lit) const {
    return lit == str_;
  }
  template <typename P> constexpr auto operator|(P parser) const {
    return or_parser<str_parser, P>{*this, parser};
  }
  strlit<N> str_;
};

template <typename L, typename R> struct or_parser {
  template <size_t M> constexpr bool operator()(strlit<M> lit) const {
    return l_(lit) || r_(lit);
  }
  template <typename P> constexpr auto operator|(P parser) const {
    return or_parser<or_parser, P>{*this, parser};
  }
  L l_;
  R r_;
};
} // namespace parse

struct derived_from_constant_wrapper : std::constant_wrapper<42> {};

struct also_derived_from_constant_wrapper : std::constant_wrapper<2> {};

struct Test {
  int value = 1;

  constexpr int operator()(int a, int b) const { return a + b + value; }

  constexpr int operator[](auto... args) const { return (value + ... + args); }

  constexpr bool operator==(const Test &) const = default;
};

template <auto Expected, std::constexpr_param C> void check(C x) {
  static_assert(std::same_as<C, std::constant_wrapper<Expected>>);
  static_assert(C::value == Expected);
  static_assert(x == Expected);
  static_assert(x.value == Expected);
}

#endif // EXAMINE_STREAM_INSERTION_ERRORS

struct foo_type {};

int main() {
#if !EXAMINE_STREAM_INSERTION_ERRORS
  auto x = std::cw<42>;
  static_assert(std::is_same_v<decltype(std::cw<42>)::value_type, int>);

  constexpr int i = 13;
  auto y = std::cw<i>;
  static_assert(std::is_same_v<decltype(std::cw<13>)::value_type, int>);

  std::cout << x << "\n";

  std::cout << x - y << "\n";
  std::cout << std::cw<42> - std::cw<13u> << "\n";
  [[maybe_unused]] auto z = std::cw<42> - std::cw<13u>;
  static_assert(std::is_same_v<decltype(std::cw<42> - std::cw<13u>),
                               std::constant_wrapper<29u>>);

  [[maybe_unused]] constexpr auto q = ~std::cw<1u> >> 10;

  std::cout << (~std::cw<1u> >> 10) << "\n";
  std::cout << +std::cw<1u> << "\n";

  constexpr my_nttp nttp;
  auto custom = std::cw<nttp>;
  // +custom; // Ill-formed!
  auto r = custom(x, y);

  std::cout << r << "\n";

  auto f = std::cw<strlit("foo")>;
  std::cout << f << "\n";

  {
    derived_from_constant_wrapper d;
    [[maybe_unused]] auto e = d + d;
    [[maybe_unused]] auto f = d + std::cw<1>;
    [[maybe_unused]] auto g = std::cw<1> + d;
  }
  {
    derived_from_constant_wrapper d;
    also_derived_from_constant_wrapper a;
    [[maybe_unused]] auto e = d + a;
  }

#if 0
    {
        constexpr std::array<int, 4> array = {1, 2, 3, 4};
        std::cw<array>[std::cw<1>];
    }
#endif

  {
    constexpr parse::str_parser p1{strlit("neg")};
    constexpr parse::str_parser p2{strlit("incr")};
    constexpr parse::str_parser p3{strlit("decr")};

    constexpr auto p = p1 | p2 | p3;

    constexpr bool matches_empty = p(strlit(""));
    static_assert(!matches_empty);
    constexpr bool matches_pos = p(strlit("pos"));
    static_assert(!matches_pos);
    constexpr bool matches_decr = p(strlit("decr"));
    static_assert(matches_decr);

    constexpr auto p_ = std::cw<p1> | std::cw<p2> | std::cw<p3>;

    constexpr bool matches_empty_ = p_(std::cw<strlit("")>);
    static_assert(!matches_empty_);
    constexpr bool matches_pos_ = p_(std::cw<strlit("pos")>);
    static_assert(!matches_pos_);
    constexpr bool matches_decr_ = p_(std::cw<strlit("decr")>);
    static_assert(matches_decr_);
  }

  {
    check<Test{}>(std::cw<Test{}>);
  }

#else

  std::cout << foo_type{};

#endif

  {
    using namespace std::my_literals;

    constexpr auto x = -1_cw + std::cw<(-1)>;
    static_assert(x == std::cw<(-2)>);
  }
  {
    constexpr int value = std::cw<42>();
    static_assert(value == 42);
  }
}
