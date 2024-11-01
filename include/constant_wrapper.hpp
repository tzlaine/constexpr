#include <type_traits>
#include <utility>

namespace std {

// forward declarations
namespace _impl {
	template <typename _T> struct _fixed_value;
};

template <_impl::_fixed_value _Value, typename _AdlType = typename decltype(_impl::_fixed_value(_Value))::_type> struct constant_wrapper;

template <class T> concept constexpr_param = requires { T::value; typename T::type; };

template <class T> concept pre_incrementable = requires(T val) {
	++val;
};

template <class T> concept post_incrementable = requires(T val) {
	++val;
};

// implementation detail to work around the language...

namespace _impl {
	template <typename _T> struct _fixed_value {
		_T _data;

		using _type = _T;

		constexpr _fixed_value(_type _v) noexcept: _data(_v) { }
	};

	template <typename _T, size_t _Extent> struct _fixed_value<_T[_Extent]> {
		_T _data[_Extent];

		using _type = _T[_Extent];

	private:
		template <size_t... Idx> constexpr _fixed_value(_T (&_in)[_Extent], std::index_sequence<Idx...>) noexcept: _data{_in[Idx]...} { }

	public:
		constexpr _fixed_value(_T (&_in)[_Extent]) noexcept: _fixed_value(_in, std::make_index_sequence<_Extent>()) { }
	};

	template <typename _T, size_t _Extent> _fixed_value(_T (&)[_Extent]) -> _fixed_value<_T[_Extent]>;
	template <typename _T> _fixed_value(_T) -> _fixed_value<_T>;

	struct operators {
		// unary operators
		template <constexpr_param T> friend constexpr auto operator+(T) noexcept -> constant_wrapper<(+T::value)> { return {}; }
		template <constexpr_param T> friend constexpr auto operator-(T) noexcept -> constant_wrapper<(-T::value)> { return {}; }
		template <constexpr_param T> friend constexpr auto operator~(T) noexcept -> constant_wrapper<(~T::value)> { return {}; }
		template <constexpr_param T> friend constexpr auto operator!(T) noexcept -> constant_wrapper<(!T::value)> { return {}; }
		template <constexpr_param T> friend constexpr auto operator&(T) noexcept -> constant_wrapper<(&T::value)> { return {}; }
		template <constexpr_param T> friend constexpr auto operator*(T) noexcept -> constant_wrapper<(*T::value)> { return {}; }

		// binary operators
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator+(L, R) noexcept -> constant_wrapper<(L::value + R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator-(L, R) noexcept -> constant_wrapper<(L::value - R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator*(L, R) noexcept -> constant_wrapper<(L::value * R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator/(L, R) noexcept -> constant_wrapper<(L::value / R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator%(L, R) noexcept -> constant_wrapper<(L::value % R::value)> { return {}; }

		template <constexpr_param L, constexpr_param R> friend constexpr auto operator<<(L, R) noexcept -> constant_wrapper<(L::value << R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator>>(L, R) noexcept -> constant_wrapper<(L::value >> R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator&(L, R) noexcept -> constant_wrapper<(L::value & R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator|(L, R) noexcept -> constant_wrapper<(L::value | R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator^(L, R) noexcept -> constant_wrapper<(L::value ^ R::value)> { return {}; }

		template <constexpr_param L, constexpr_param R> friend constexpr auto operator&&(L, R) noexcept -> constant_wrapper<(L::value && R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator||(L, R) noexcept -> constant_wrapper<(L::value || R::value)> { return {}; }

		// comparisons
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator<=>(L, R) noexcept -> constant_wrapper<(L::value <=> R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator<(L, R) noexcept -> constant_wrapper<(L::value < R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator<=(L, R) noexcept -> constant_wrapper<(L::value <= R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator==(L, R) noexcept -> constant_wrapper<(L::value == R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator!=(L, R) noexcept -> constant_wrapper<(L::value != R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator>(L, R) noexcept -> constant_wrapper<(L::value > R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator>=(L, R) noexcept -> constant_wrapper<(L::value >= R::value)> { return {}; }

		template <constexpr_param L, constexpr_param R> friend constexpr auto operator,(L, R) noexcept -> constant_wrapper<operator,(L::value, R::value)> { return {}; }
		template <constexpr_param L, constexpr_param R> friend constexpr auto operator->*(L, R) noexcept -> constant_wrapper<operator->*(L::value, R::value)> { return {}; }

		// access
		template <constexpr_param T, constexpr_param... Args> constexpr auto operator()(this T, Args...) noexcept requires requires(T::type x, Args...) { x(Args::value...); } {
			return constant_wrapper<(T::value(Args::value...))>{};
		}
		template <constexpr_param T, constexpr_param... Args> constexpr auto operator[](this T, Args...) noexcept -> constant_wrapper<(T::value[Args::value...])> { return {}; }

		// semi modifying (needs to be done with lambda, and requires, otherwise it will give ugly error, and on on clang inline `++copy(x)` gives error)
		template <constexpr_param T> constexpr auto operator++(this T) noexcept requires requires(T::type x) { ++x; } {
			return constant_wrapper<[] { auto c = T::value; return ++c; }()>{};
		}
		template <constexpr_param T> constexpr auto operator++(this T, int) noexcept requires requires(T::type x) { x++; } {
			return constant_wrapper<[] { auto c = T::value; return ++c; }()>{};
		}

		template <constexpr_param T> constexpr auto operator--(this T) noexcept requires requires(T::type x) { --x; } {
			return constant_wrapper<[] { auto c = T::value; return --c; }()>{};
		}
		template <constexpr_param T> constexpr auto operator--(this T, int) noexcept requires requires(T::type x) { x--; } {
			return constant_wrapper<[] { auto c = T::value; return c--; }()>{};
		}

		template <constexpr_param T, constexpr_param R> constexpr auto operator+=(this T, R) noexcept requires requires(T::type x) { x += R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v += R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator-=(this T, R) noexcept requires requires(T::type x) { x -= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v -= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator*=(this T, R) noexcept requires requires(T::type x) { x *= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v *= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator/=(this T, R) noexcept requires requires(T::type x) { x /= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v /= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator%=(this T, R) noexcept requires requires(T::type x) { x %= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v %= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator&=(this T, R) noexcept requires requires(T::type x) { x &= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v &= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator|=(this T, R) noexcept requires requires(T::type x) { x |= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v |= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator^=(this T, R) noexcept requires requires(T::type x) { x ^= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v ^= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator<<=(this T, R) noexcept requires requires(T::type x) { x <<= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v <<= R::value; }()>{};
		}
		template <constexpr_param T, constexpr_param R> constexpr auto operator>>=(this T, R) noexcept requires requires(T::type x) { x >>= R::value; } {
			return constant_wrapper<[] { auto v = T::value; return v >>= R::value; }()>{};
		}
	};
} // namespace _impl

template <_impl::_fixed_value _Value, typename _AdlType> struct constant_wrapper: _impl::operators {
	static constexpr const auto & value = _Value._data;
	using type = typename decltype(_Value)::_type;
	using value_type = type;

	constexpr operator decltype(auto)() const noexcept {
		return value;
	}

	using _impl::operators::operator();

	constexpr decltype(auto) operator()() const noexcept {
		return value;
	}
};

template <_impl::_fixed_value _Value> constexpr auto cw = constant_wrapper<_Value>{};

inline namespace literals {

	template <_impl::_fixed_value _Value> consteval auto operator"" _cw() { return constant_wrapper<_Value>{}; }

	// TODO parsing
	// template <typename _Value> constexpr auto operator"" _cw() { return constant_wrapper<_Value>{}; }

} // namespace literals

} // namespace std
