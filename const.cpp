#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string_view>


#define EXAMINE_STREAM_INSERTION_ERRORS 0

#if !EXAMINE_STREAM_INSERTION_ERRORS

namespace std {
  template<auto X, class T = remove_cvref_t<decltype(X)>>
    struct constexpr_v;

  // exposition only
  template <class T>
    concept constexpr_param =
      !is_member_pointer_v<decltype(&T::value)> && requires { typename constexpr_v<T::value>; };
  template <class T>
    concept derived_from_constexpr = derived_from<T, constexpr_v<T::value>>;
  template <class T, class SelfT>
    concept lhs_constexpr_param = constexpr_param<T> && (derived_from<T, SelfT> || !derived_from_constexpr<T>);

  template<auto X, class T>
  struct constexpr_v {
    using value_type = T;
    using type = constexpr_v;

    constexpr operator value_type() const { return X; }
    static constexpr value_type value = X;

#define LEWG_SAYS_SO 1
#if LEWG_SAYS_SO
    template <constexpr_param U>
      constexpr constexpr_v<(X = U::value)> operator=(U) const { return {}; }
#endif

    template<auto Y = X>
      constexpr constexpr_v<+Y> operator+() const { return {}; }
    template<auto Y = X>
      constexpr constexpr_v<-Y> operator-() const { return {}; }
    template<auto Y = X>
      constexpr constexpr_v<~Y> operator~() const { return {}; }
    template<auto Y = X>
      constexpr constexpr_v<!Y> operator!() const { return {}; }
    template<auto Y = X>
      constexpr constexpr_v<&Y> operator&() const { return {}; }
    template<auto Y = X>
      constexpr constexpr_v<*Y> operator*() const { return {}; }

    template<class... Args>
      constexpr constexpr_v<X(Args::value...)> operator()(Args... args) const { return {}; }
    template<class... Args>
      constexpr constexpr_v<X[Args::value...]> operator[](Args... args) const { return {}; }

    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value + V::value> operator+(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value - V::value> operator-(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value * V::value> operator*(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value / V::value> operator/(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value % V::value> operator%(U, V) { return {}; }

    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value << V::value)> operator<<(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value >> V::value)> operator>>(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value & V::value> operator&(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value | V::value> operator|(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value ^ V::value> operator^(U, V) { return {}; }

    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value && V::value> operator&&(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<U::value || V::value> operator||(U, V) { return {}; }

    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value <=> V::value)> operator<=>(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value == V::value)> operator==(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value != V::value)> operator!=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value < V::value)> operator<(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value > V::value)> operator>(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value <= V::value)> operator<=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value >= V::value)> operator>=(U, V) { return {}; }

    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value, V::value)> operator,(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value ->* V::value)> operator->*(U, V) { return {}; }

#if LEWG_SAYS_SO
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value += V::value)> operator+=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value -= V::value)> operator-=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value *= V::value)> operator*=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value /= V::value)> operator/=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value %= V::value)> operator%=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value &= V::value)> operator&=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value |= V::value)> operator|=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value ^= V::value)> operator^=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value <<= V::value)> operator<<=(U, V) { return {}; }
    template <lhs_constexpr_param<type> U, constexpr_param V>
      friend constexpr constexpr_v<(U::value >>= V::value)> operator>>=(U, V) { return {}; }
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

// g++-13 ./const.cpp -o const.bin -DDEBUG -std=c++23 && ./const.bin

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
    x.f(std::c_<2>);
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

struct Test
{
    int value = 1;

    constexpr int operator()(int a, int b) const { return a + b + value; }

    constexpr int operator[](auto... args) const
    {
        return (value + ... + args);
    }

    constexpr bool operator==(const Test &) const = default;
};

template<auto Expected, std::constexpr_param C>
void check(C x)
{
    static_assert(std::same_as<C, std::constexpr_v<Expected>>);
    static_assert(C::value == Expected);
    static_assert(x == Expected);
    static_assert(x.value == Expected);
}

#endif // EXAMINE_STREAM_INSERTION_ERRORS

struct foo_type
{};

int main()
{
#if !EXAMINE_STREAM_INSERTION_ERRORS
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

#if 0
    {
        constexpr std::array<int, 4> array = {1, 2, 3, 4};
        std::c_<array>[std::c_<1>];
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

        constexpr auto p_ = std::c_<p1> | std::c_<p2> | std::c_<p3>;

        constexpr bool matches_empty_ = p_(std::c_<strlit("")>);
        static_assert(!matches_empty_);
        constexpr bool matches_pos_ = p_(std::c_<strlit("pos")>);
        static_assert(!matches_pos_);
        constexpr bool matches_decr_ = p_(std::c_<strlit("decr")>);
        static_assert(matches_decr_);
    }

    {
        check<Test{}>(std::c_<Test{}>);
    }

#else

    std::cout << foo_type{};

#endif
}
