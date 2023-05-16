#include <algorithm>
#include <cassert>
#include <iostream>
#include <string_view>


namespace std {
  template<class T, T X>
    requires same_as<remove_cvref_t<T>, T>
      struct constexpr_v;

  // exposition only
  template<class T>
    constexpr bool not_constexpr_v = true;
  template<class T, T X>
    constexpr bool not_constexpr_v<constexpr_v<T, X>> = false;

  template<class T, T X>
    requires same_as<remove_cvref_t<T>, T>
  struct constexpr_v
  {
    static constexpr T value = X;

    using value_type = T;
    using type = constexpr_v;

    constexpr operator value_type() const { return X; }

    constexpr auto operator+() const
      requires requires { constexpr_v<decltype(+X), +X>{}; }
        { return constexpr_v<decltype(+X), +X>{}; }
    constexpr auto operator-() const
      requires requires { constexpr_v<decltype(-X), -X>{}; }
        { return constexpr_v<decltype(-X), -X>{}; }
    constexpr auto operator~() const
      requires requires { constexpr_v<decltype(~X), ~X>{}; }
        { return constexpr_v<decltype(~X), ~X>{}; }
    constexpr auto operator!() const
      requires requires { constexpr_v<decltype(!X), !X>{}; }
        { return constexpr_v<decltype(!X), !X>{}; }
    constexpr auto operator&() const
      requires requires { constexpr_v<decltype(&X), &X>{}; }
        { return constexpr_v<decltype(&X), &X>{}; }
    constexpr auto operator*() const
      requires requires { constexpr_v<decltype(*X), *X>{}; }
        { return constexpr_v<decltype(*X), *X>{}; }

    template<class U>
      friend constexpr constexpr_v<decltype(X << U::value), (X << U::value)>
        operator<<(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value << X), (U::value << X)>
          operator<<(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X >> U::value), (X >> U::value)>
        operator>>(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value >> X), (U::value >> X)>
          operator>>(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X * U::value), (X * U::value)>
        operator*(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value * X), (U::value * X)>
          operator*(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X / U::value), (X / U::value)>
        operator/(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value / X), (U::value / X)>
          operator/(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X % U::value), (X % U::value)>
        operator%(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value % X), (U::value % X)>
          operator%(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X + U::value), (X + U::value)>
        operator+(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value + X), (U::value + X)>
          operator+(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X - U::value), (X - U::value)>
        operator-(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value - X), (U::value - X)>
          operator-(U, constexpr_v) { return {}; }

    template<class U>
      friend constexpr constexpr_v<decltype(X < U::value), (X < U::value)>
        operator<(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value < X), (U::value < X)>
          operator<(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X > U::value), (X > U::value)>
        operator>(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value > X), (U::value > X)>
          operator>(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X <= U::value), (X <= U::value)>
        operator<=(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value <= X), (U::value <= X)>
          operator<=(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X >= U::value), (X >= U::value)>
        operator>=(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value >= X), (U::value >= X)>
          operator>=(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X == U::value), (X == U::value)>
        operator==(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value == X), (U::value == X)>
          operator==(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X != U::value), (X != U::value)>
        operator!=(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value != X), (U::value != X)>
          operator!=(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X <=> U::value), (X <=> U::value)>
        operator<=>(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value <=> X), (U::value <=> X)>
          operator<=>(U, constexpr_v) { return {}; }

    template<class U>
      friend constexpr constexpr_v<decltype(X && U::value), (X && U::value)>
        operator&&(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value && X), (U::value && X)>
          operator&&(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X || U::value), (X || U::value)>
        operator||(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value || X), (U::value || X)>
          operator||(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X & U::value), (X & U::value)>
        operator&(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value & X), (U::value & X)>
          operator&(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X | U::value), (X | U::value)>
        operator|(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value | X), (U::value | X)>
          operator|(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X ^ U::value), (X ^ U::value)>
        operator^(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value ^ X), (U::value ^ X)>
          operator^(U, constexpr_v) { return {}; }

    template<class U>
      friend constexpr constexpr_v<decltype(X , U::value), (X , U::value)>
        operator,(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value , X), (U::value , X)>
          operator,(U, constexpr_v) { return {}; }
    template<class U>
      friend constexpr constexpr_v<decltype(X ->* U::value), (X ->* U::value)>
        operator->*(constexpr_v, U) { return {}; }
    template<class U>
      requires not_constexpr_v<U>
        friend constexpr constexpr_v<decltype(U::value ->* X), (U::value ->* X)>
          operator->*(U, constexpr_v) { return {}; }

    template<class... Args>
      constexpr auto operator()(Args... args) const
        -> constexpr_v<decltype(X(Args::value...)), X(Args::value...)>
          { return {}; }
#if 0
    template<class... Args>
      constexpr auto operator[](Args... args) const
        -> constexpr_v<decltype(X[Args::value...]), X[Args::value...]>
          { return {}; }
#endif
  };

  template<auto X>
    inline constexpr constexpr_v<remove_const_t<decltype(X)>, X> c_{};

  template<typename T, typename ValueType>
    struct well_formed_static_constexpr_value {
      static constexpr ValueType value = T::value;
    };
  template<typename T>
    struct well_formed_static_constexpr_value<T, void> {
      static constexpr auto value = T::value;
    };

  template<typename T, typename ValueType = void>
    concept constexpr_value =
      requires { well_formed_static_constexpr_value<T, ValueType>::value; };
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
    T re, im;
};

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
    x.f(std::c_<3.0>);
    x.f(std::c_<4.f>);
    x.f(std::c_<foo>);
    x.f(std::c_<my_complex(1.f, 1.f)>);
}

struct my_type { constexpr static int value = 42; };

template<typename T, T Value>
struct my_constant
{
    static constexpr T value = Value;
    
    // Other API ...
};

auto plus(std::constexpr_value<int> auto x, std::constexpr_value<int> auto y)
{ return std::c_<x.value + y.value>; }

void h()
{
    // All results are equal to std::c_<1>.
    auto result_1 = plus(std::c_<0>, std::c_<1>);                        // Ok.
    auto result_2 = plus(std::c_<0>, std::integral_constant<int, 1>{});  // Also ok.
    auto result_3 = plus(std::c_<0>, my_constant<int, 1>{});             // Still ok.

    assert(result_1 == 1);
    assert(result_2 == 1);
    assert(result_3 == 1);
}

auto operator+(std::constexpr_value auto x, std::constexpr_value auto y)
    -> decltype(std::c_<decltype(x)::value + decltype(y)::value>)
{ return std::c_<x.value + y.value>; }

void i()
{
    // Equal to std::c_<1>.
    auto result = std::integral_constant<int, 0>{} + my_constant<int, 1>{};

    assert(result == 1);
}

namespace parse {

    template<typename L, typename R>
    struct or_parser;

    template<size_t N>
    struct str_parser
    {
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
                  std::constexpr_v<unsigned int, 29u>>);

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
 
     std::cout << my_type{} - std::c_<42> << "\n";
 
     ::h();
     ::i();

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
