#include <type_traits>
#include <utility>

namespace std {

namespace exposition_only {
  template<typename T>
  struct fixed_value; // exposition only
};

template<exposition_only::fixed_value Value,
        typename adl_type = typename decltype(exposition_only::fixed_value(Value))::type> // exposition only
struct constant_wrapper;

template<class T>
concept constexpr_param = requires { typename constant_wrapper<T::value>; }; // exposition only

namespace exposition_only {
  template<typename T>
  struct fixed_value { // exposition only
    T data;

    using type = T;

    constexpr fixed_value(type v) noexcept: data(v) { }
  };

  template<typename T, size_t Extent>
  struct fixed_value<T[Extent]> { // exposition only
    T data[Extent];

    using type = T[Extent];

  private:
    template<size_t... Idx>
    constexpr fixed_value(T (&arr)[Extent], std::index_sequence<Idx...>) noexcept: data{arr[Idx]...} { }

  public:
    constexpr fixed_value(T (&arr)[Extent]) noexcept: fixed_value(arr, std::make_index_sequence<Extent>()) { }
  };

  template<typename T, size_t Extent>
  fixed_value(T (&)[Extent]) -> fixed_value<T[Extent]>; // exposition only
  template<typename T>
  fixed_value(T) -> fixed_value<T>;                     // exposition only

  struct operators { // exposition only
    // unary operators
    template<constexpr_param T>
    friend constexpr auto operator+(T) noexcept -> constant_wrapper<(+T::value)> { return {}; }
    template<constexpr_param T>
    friend constexpr auto operator-(T) noexcept -> constant_wrapper<(-T::value)> { return {}; }
    template<constexpr_param T>
    friend constexpr auto operator~(T) noexcept -> constant_wrapper<(~T::value)> { return {}; }
    template<constexpr_param T>
    friend constexpr auto operator!(T) noexcept -> constant_wrapper<(!T::value)> { return {}; }
    template<constexpr_param T>
    friend constexpr auto operator&(T) noexcept -> constant_wrapper<(&T::value)> { return {}; }
    template<constexpr_param T>
    friend constexpr auto operator*(T) noexcept -> constant_wrapper<(*T::value)> { return {}; }

    // binary operators
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator+(L, R) noexcept -> constant_wrapper<(L::value + R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator-(L, R) noexcept -> constant_wrapper<(L::value - R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator*(L, R) noexcept -> constant_wrapper<(L::value * R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator/(L, R) noexcept -> constant_wrapper<(L::value / R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator%(L, R) noexcept -> constant_wrapper<(L::value % R::value)> { return {}; }

    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator<<(L, R) noexcept -> constant_wrapper<(L::value << R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator>>(L, R) noexcept -> constant_wrapper<(L::value >> R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator&(L, R) noexcept -> constant_wrapper<(L::value & R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator|(L, R) noexcept -> constant_wrapper<(L::value | R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator^(L, R) noexcept -> constant_wrapper<(L::value ^ R::value)> { return {}; }

    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator&&(L, R) noexcept -> constant_wrapper<(L::value && R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator||(L, R) noexcept -> constant_wrapper<(L::value || R::value)> { return {}; }

    // comparisons
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator<=>(L, R) noexcept -> constant_wrapper<(L::value <=> R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator<(L, R) noexcept -> constant_wrapper<(L::value < R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator<=(L, R) noexcept -> constant_wrapper<(L::value <= R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator==(L, R) noexcept -> constant_wrapper<(L::value == R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator!=(L, R) noexcept -> constant_wrapper<(L::value != R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator>(L, R) noexcept -> constant_wrapper<(L::value > R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator>=(L, R) noexcept -> constant_wrapper<(L::value >= R::value)> { return {}; }

    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator,(L, R) noexcept -> constant_wrapper<operator,(L::value, R::value)> { return {}; }
    template<constexpr_param L, constexpr_param R>
    friend constexpr auto operator->*(L, R) noexcept -> constant_wrapper<operator->*(L::value, R::value)> { return {}; }

    // call and index
    template<constexpr_param T, constexpr_param... Args>
    constexpr auto operator()(this T, Args...) noexcept requires requires(T::value_type x, Args...) { x(Args::value...); } {
      return constant_wrapper<(T::value(Args::value...))>{};
    }
    template<constexpr_param T, constexpr_param... Args>
    constexpr auto operator[](this T, Args...) noexcept -> constant_wrapper<(T::value[Args::value...])> { return {}; }

    // pseudo-mutators
    template<constexpr_param T>
    constexpr auto operator++(this T) noexcept requires requires(T::value_type x) { ++x; } {
      return constant_wrapper<[] { auto c = T::value; return ++c; }()>{};
    }
    template<constexpr_param T>
    constexpr auto operator++(this T, int) noexcept requires requires(T::value_type x) { x++; } {
      return constant_wrapper<[] { auto c = T::value; return ++c; }()>{};
    }

    template<constexpr_param T>
    constexpr auto operator--(this T) noexcept requires requires(T::value_type x) { --x; } {
      return constant_wrapper<[] { auto c = T::value; return --c; }()>{};
    }
    template<constexpr_param T>
    constexpr auto operator--(this T, int) noexcept requires requires(T::value_type x) { x--; } {
      return constant_wrapper<[] { auto c = T::value; return c--; }()>{};
    }

    template<constexpr_param T, constexpr_param R>
    constexpr auto operator+=(this T, R) noexcept requires requires(T::value_type x) { x += R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v += R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator-=(this T, R) noexcept requires requires(T::value_type x) { x -= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v -= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator*=(this T, R) noexcept requires requires(T::value_type x) { x *= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v *= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator/=(this T, R) noexcept requires requires(T::value_type x) { x /= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v /= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator%=(this T, R) noexcept requires requires(T::value_type x) { x %= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v %= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator&=(this T, R) noexcept requires requires(T::value_type x) { x &= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v &= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator|=(this T, R) noexcept requires requires(T::value_type x) { x |= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v |= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator^=(this T, R) noexcept requires requires(T::value_type x) { x ^= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v ^= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator<<=(this T, R) noexcept requires requires(T::value_type x) { x <<= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v <<= R::value; }()>{};
    }
    template<constexpr_param T, constexpr_param R>
    constexpr auto operator>>=(this T, R) noexcept requires requires(T::value_type x) { x >>= R::value; } {
      return constant_wrapper<[] { auto v = T::value; return v >>= R::value; }()>{};
    }
  };
}

template<exposition_only::fixed_value Value, typename adl_type>
struct constant_wrapper: exposition_only::operators {
  static constexpr const auto & value = Value.data;
  using type = constant_wrapper;
  using value_type = typename decltype(Value)::type;

  template<constexpr_param R>
  constexpr auto operator=(R) const noexcept requires requires(value_type x) { x = R::value; } {
    return constant_wrapper<[] { auto v = value; return v = R::value; }()>{};
  }

  constexpr operator decltype(auto)() const noexcept { return value; }
  constexpr decltype(auto) operator()() const noexcept { return value; }

  using exposition_only::operators::operator();
};

template<exposition_only::fixed_value Value>
constexpr auto cw = constant_wrapper<Value>{};

} // namespace std
