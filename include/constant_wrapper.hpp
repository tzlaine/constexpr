#include <type_traits>
#include <utility>


namespace std {

namespace exposition_only {
  template<typename T>
  struct cw_fixed_value; // exposition only
}

template<exposition_only::cw_fixed_value X,
         typename = typename decltype(exposition_only::cw_fixed_value(X))::type> // exposition only
struct constant_wrapper;

template<class T>
concept consteval_param = requires { typename constant_wrapper<T::value>; }; // exposition only

namespace exposition_only {
  template<typename T>
  struct cw_fixed_value { // exposition only
    using type = T;
    consteval cw_fixed_value(type v) noexcept: data(v) { }
    T data;
  };

  template<typename T, size_t Extent>
  struct cw_fixed_value<T[Extent]> { // exposition only
    using type = T[Extent];
    consteval cw_fixed_value(T (&arr)[Extent]) noexcept: cw_fixed_value(arr, std::make_index_sequence<Extent>()) { }
    T data[Extent];

  private:
    template<size_t... Idx>
    consteval cw_fixed_value(T (&arr)[Extent], std::index_sequence<Idx...>) noexcept: data{arr[Idx]...} { }
  };

  template<typename T, size_t Extent>
  cw_fixed_value(T (&)[Extent]) -> cw_fixed_value<T[Extent]>; // exposition only
  template<typename T>
  cw_fixed_value(T) -> cw_fixed_value<T>;                     // exposition only

  struct cw_operators { // exposition only
    // unary operators
    template<consteval_param T>
      friend consteval auto operator+(T) noexcept -> constant_wrapper<(+T::value)> { return {}; }
    template<consteval_param T>
      friend consteval auto operator-(T) noexcept -> constant_wrapper<(-T::value)> { return {}; }
    template<consteval_param T>
      friend consteval auto operator~(T) noexcept -> constant_wrapper<(~T::value)> { return {}; }
    template<consteval_param T>
      friend consteval auto operator!(T) noexcept -> constant_wrapper<(!T::value)> { return {}; }
    template<consteval_param T>
      friend consteval auto operator&(T) noexcept -> constant_wrapper<(&T::value)> { return {}; }
    template<consteval_param T>
      friend consteval auto operator*(T) noexcept -> constant_wrapper<(*T::value)> { return {}; }

    // binary operators
    template<consteval_param L, consteval_param R>
      friend consteval auto operator+(L, R) noexcept -> constant_wrapper<(L::value + R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator-(L, R) noexcept -> constant_wrapper<(L::value - R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator*(L, R) noexcept -> constant_wrapper<(L::value * R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator/(L, R) noexcept -> constant_wrapper<(L::value / R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator%(L, R) noexcept -> constant_wrapper<(L::value % R::value)> { return {}; }

    template<consteval_param L, consteval_param R>
      friend consteval auto operator<<(L, R) noexcept -> constant_wrapper<(L::value << R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator>>(L, R) noexcept -> constant_wrapper<(L::value >> R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator&(L, R) noexcept -> constant_wrapper<(L::value & R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator|(L, R) noexcept -> constant_wrapper<(L::value | R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator^(L, R) noexcept -> constant_wrapper<(L::value ^ R::value)> { return {}; }

    template<consteval_param L, consteval_param R>
      friend consteval auto operator&&(L, R) noexcept -> constant_wrapper<(L::value && R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator||(L, R) noexcept -> constant_wrapper<(L::value || R::value)> { return {}; }

    // comparisons
    template<consteval_param L, consteval_param R>
      friend consteval auto operator<=>(L, R) noexcept -> constant_wrapper<(L::value <=> R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator<(L, R) noexcept -> constant_wrapper<(L::value < R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator<=(L, R) noexcept -> constant_wrapper<(L::value <= R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator==(L, R) noexcept -> constant_wrapper<(L::value == R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator!=(L, R) noexcept -> constant_wrapper<(L::value != R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator>(L, R) noexcept -> constant_wrapper<(L::value > R::value)> { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator>=(L, R) noexcept -> constant_wrapper<(L::value >= R::value)> { return {}; }

    template<consteval_param L, consteval_param R>
      friend consteval auto operator,(L, R) noexcept -> constant_wrapper<operator,(L::value, R::value)>
         { return {}; }
    template<consteval_param L, consteval_param R>
      friend consteval auto operator->*(L, R) noexcept -> constant_wrapper<operator->*(L::value, R::value)>
        { return {}; }

    // call and index
    template<consteval_param T, consteval_param... Args>
      consteval auto operator()(this T, Args...) noexcept
        requires requires(T::value_type x, Args...) { x(Args::value...); }
          { return constant_wrapper<(T::value(Args::value...))>{}; }
    template<consteval_param T, consteval_param... Args>
      consteval auto operator[](this T, Args...) noexcept -> constant_wrapper<(T::value[Args::value...])>
        { return {}; }

    // pseudo-mutators
    template<consteval_param T>
      consteval auto operator++(this T) noexcept requires requires(T::value_type x) { ++x; }
        { return constant_wrapper<[] { auto c = T::value; return ++c; }()>{}; }
    template<consteval_param T>
      consteval auto operator++(this T, int) noexcept requires requires(T::value_type x) { x++; }
        { return constant_wrapper<[] { auto c = T::value; return c++; }()>{}; }

    template<consteval_param T>
      consteval auto operator--(this T) noexcept requires requires(T::value_type x) { --x; }
        { return constant_wrapper<[] { auto c = T::value; return --c; }()>{}; }
    template<consteval_param T>
      consteval auto operator--(this T, int) noexcept requires requires(T::value_type x) { x--; }
        { return constant_wrapper<[] { auto c = T::value; return c--; }()>{}; }

    template<consteval_param T, consteval_param R>
      consteval auto operator+=(this T, R) noexcept requires requires(T::value_type x) { x += R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v += R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator-=(this T, R) noexcept requires requires(T::value_type x) { x -= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v -= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator*=(this T, R) noexcept requires requires(T::value_type x) { x *= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v *= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator/=(this T, R) noexcept requires requires(T::value_type x) { x /= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v /= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator%=(this T, R) noexcept requires requires(T::value_type x) { x %= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v %= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator&=(this T, R) noexcept requires requires(T::value_type x) { x &= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v &= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator|=(this T, R) noexcept requires requires(T::value_type x) { x |= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v |= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator^=(this T, R) noexcept requires requires(T::value_type x) { x ^= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v ^= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator<<=(this T, R) noexcept requires requires(T::value_type x) { x <<= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v <<= R::value; }()>{}; }
    template<consteval_param T, consteval_param R>
      consteval auto operator>>=(this T, R) noexcept requires requires(T::value_type x) { x >>= R::value; }
        { return constant_wrapper<[] { auto v = T::value; return v >>= R::value; }()>{}; }
  };
}

template<exposition_only::cw_fixed_value X, typename>
struct constant_wrapper: exposition_only::cw_operators {
  static constexpr const auto & value = X.data;
  using type = constant_wrapper;
  using value_type = typename decltype(X)::type;

  template<consteval_param R>
    consteval auto operator=(R) const noexcept requires requires(value_type x) { x = R::value; }
      { return constant_wrapper<[] { auto v = value; return v = R::value; }()>{}; }

  consteval operator decltype(auto)() const noexcept { return value; }
  consteval decltype(auto) operator()() const noexcept requires (!std::invocable<value_type>) { return value; }

  using exposition_only::cw_operators::operator();
};

template<exposition_only::cw_fixed_value X>
  constinit auto cw = constant_wrapper<X>{};

} // namespace std
