#include <algorithm>
#include <cassert>
#include <iostream>
#include <string_view>


namespace std {
  template<auto X, class T = remove_const_t<decltype(X)>>
    struct constexpr_v;

  // exposition only
  template <class T>
    concept constexpr_value =
      requires { typename constexpr_v<T::value>; } and not is_member_pointer_v<decltype(&T::value)>;
  template <class T>
    concept derived_from_constexpr = derived_from<T, constexpr_v<T::value>>;
  template <class T, class SelfT>
    concept lhs_constexpr_value = constexpr_value<T> and (derived_from<T, SelfT> or not derived_from_constexpr<T>);

  template<auto X, class>
  struct constexpr_v {
    using value_type = decltype(X);
    using type = constexpr_v;

    constexpr operator value_type() const { return X; }
    static constexpr value_type value = X;

#define LEWG_SAYS_SO 1
#if LEWG_SAYS_SO
    template <constexpr_value T>
      constexpr constexpr_v<X = T::value> operator=(T) const { return {}; }
#endif

    template<auto Y = X>
      constexpr auto operator+() const -> constexpr_v<+Y> { return {}; }
    template<auto Y = X>
      constexpr auto operator-() const -> constexpr_v<-Y> { return {}; }
    template<auto Y = X>
      constexpr auto operator~() const -> constexpr_v<~Y> { return {}; }
    template<auto Y = X>
      constexpr auto operator!() const -> constexpr_v<!Y> { return {}; }
    template<auto Y = X>
      constexpr auto operator&() const -> constexpr_v<&Y> { return {}; }
    template<auto Y = X>
      constexpr auto operator*() const -> constexpr_v<*Y> { return {}; }

    template<class... Args>
      constexpr auto operator()(Args... args) const -> constexpr_v<X(Args::value...)> { return {}; }
#if 0
    template<class... Args>
      constexpr auto operator[](Args... args) const -> constexpr_v<X[Args::value...]> { return {}; }
#endif

    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value + U::value> operator+(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value - U::value> operator-(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value * U::value> operator*(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value / U::value> operator/(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value % U::value> operator%(T, U) { return {}; }

    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value << U::value)> operator<<(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value >> U::value)> operator>>(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value & U::value> operator&(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value | U::value> operator|(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value ^ U::value> operator^(T, U) { return {}; }

    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value && U::value> operator&&(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<T::value || U::value> operator||(T, U) { return {}; }

    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value <=> U::value)> operator<=>(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value == U::value)> operator==(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value != U::value)> operator!=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value < U::value)> operator<(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value > U::value)> operator>(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value <= U::value)> operator<=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value >= U::value)> operator>=(T, U) { return {}; }

    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value, U::value)> operator,(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value ->* U::value)> operator->*(T, U) { return {}; }

#if LEWG_SAYS_SO
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value += U::value)> operator+=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value -= U::value)> operator-=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value *= U::value)> operator*=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value /= U::value)> operator/=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value %= U::value)> operator%=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value &= U::value)> operator&=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value |= U::value)> operator|=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value ^= U::value)> operator^=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value <<= U::value)> operator<<=(T, U) { return {}; }
    template <lhs_constexpr_value<type> T, constexpr_value U>
      friend constexpr constexpr_v<(T::value >>= U::value)> operator>>=(T, U) { return {}; }
#endif
  };

  template<auto X>
    inline constexpr constexpr_v<X> c_{};
}

// increment and decrement operators intentionally left out
// operator OP= intentionally left out

struct my_nttp
{
    template<typename T, typename U>
    constexpr auto operator()(T t, U u) const
    {
        return t + u;
    }
};

template<size_t N>
struct strlit
{
    constexpr strlit(char const (&str)[N]) { std::copy_n(str, N, bytes_); }

    template<size_t M>
    constexpr bool operator==(strlit<M> rhs) const
    {
        return std::ranges::equal(bytes_, rhs.bytes_);
    }

    friend std::ostream & operator<<(std::ostream & os, strlit l)
    {
        assert(!l.bytes_[N - 1] && "bytes_ must be null-terminated");
        return os.write(l.bytes_, N - 1);
    }

    char bytes_[N];
};

// g++ ./const.cpp -o const.bin -DDEBUG -std=c++23 && ./const.bin

template<typename T>
struct my_complex
{
    constexpr my_complex(float r, float i) : re{r}, im{i} {}
    T re, im;
};

#if __clang__
template<typename T>
my_complex(T, T) -> my_complex<T>;
#endif

template<typename T>
struct X
{
    void f(auto c) {}
};

inline constexpr short foo = 2;

template<typename T>
void g(X<T> x)
{
    x.f(std::c_<1>);
    x.f(std::c_<2uz>);
#if !__clang__
    x.f(std::c_<3.0>);
    x.f(std::c_<4.f>);
#endif
    x.f(std::c_<foo>);
    x.f(std::c_<my_complex(1.f, 1.f)>);
}

namespace parse {

    template<typename L, typename R>
    struct or_parser;

    template<size_t N>
    struct str_parser
    {
        constexpr explicit str_parser(strlit<N> s) : str_{s} {}
        template<size_t M>
        constexpr bool operator()(strlit<M> lit) const
        {
            return lit == str_;
        }
        template<typename P>
        constexpr auto operator|(P parser) const
        {
            return or_parser<str_parser, P>{*this, parser};
        }
        strlit<N> str_;
    };

    template<typename L, typename R>
    struct or_parser
    {
        template<size_t M>
        constexpr bool operator()(strlit<M> lit) const
        {
            return l_(lit) || r_(lit);
        }
        template<typename P>
        constexpr auto operator|(P parser) const
        {
            return or_parser<or_parser, P>{*this, parser};
        }
        L l_;
        R r_;
    };
}

struct derived_from_constexpr_v : std::constexpr_v<42>
{};

struct also_derived_from_constexpr_v : std::constexpr_v<2>
{};

int main()
{
    auto x = std::c_<42>;
    static_assert(std::is_same_v<decltype(std::c_<42>)::value_type, int>);

    constexpr int i = 13;
    auto y = std::c_<i>;
    static_assert(std::is_same_v<decltype(std::c_<13>)::value_type, int>);

    std::cout << x << "\n";

    std::cout << x - y << "\n";
    std::cout << std::c_<42> - std::c_<13u> << "\n";
    auto z = std::c_<42> - std::c_<13u>;
    static_assert(std::is_same_v<
                  decltype(std::c_<42> - std::c_<13u>),
                  std::constexpr_v<29u>>);

    constexpr auto q = ~std::c_<1u> >> 10;

    std::cout << (~std::c_<1u> >> 10) << "\n";
    std::cout << +std::c_<1u> << "\n";

    constexpr my_nttp nttp;
    auto custom = std::c_<nttp>;
    // +custom; // Ill-formed!
    auto r = custom(x, y);

    std::cout << r << "\n";

    auto f = std::c_<strlit("foo")>;
    std::cout << f << "\n";

    {
        derived_from_constexpr_v d;
        auto e = d + d;
        auto f = d + std::c_<1>;
        auto g = std::c_<1> + d;
    }
    {
        derived_from_constexpr_v d;
        also_derived_from_constexpr_v a;
        auto e = d + a;
    }

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

        constexpr auto p_ = std::c_<p1> | std::c_<p2> | std::c_<p3>;

        constexpr bool matches_empty_ = p_(std::c_<strlit("")>);
        static_assert(!matches_empty_);
        constexpr bool matches_pos_ = p_(std::c_<strlit("pos")>);
        static_assert(!matches_pos_);
        constexpr bool matches_decr_ = p_(std::c_<strlit("decr")>);
        static_assert(matches_decr_);
    }
}
