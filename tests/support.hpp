#ifndef SUPPORT_HPP
#define SUPPORT_HPP

struct type_sink {};

template <typename T> struct type_t {
  consteval operator type_sink() const noexcept { return {}; }
  consteval friend bool operator==(type_t, type_t) noexcept { return true; }
};

consteval bool operator==(type_sink, type_sink) noexcept { return false; }

template <auto V> constexpr auto type_of = type_t<decltype(V)>{};
template <typename T> constexpr auto type = type_t<T>{};

template <typename> struct identify;

#endif