export module numerical;

import <type_traits>;
import <concepts>;
import <numbers>;
import <limits>;
import <vector>;
import <ranges>;
import <algorithm>;
import <numeric>;
import <execution>;
import from_to;

/*****************************************************************************************
*
*                                  numerical constants
*
*****************************************************************************************/
export template<class T> constexpr T minimum = std::template numeric_limits<T>::min();
export template<class T> constexpr const T maximum = std::template numeric_limits<T>::max();
export template<std::floating_point T> constexpr const T epsilon = std::template numeric_limits<T>::epsilon();
export constexpr float pi32 = std::numbers::pi_v<float>;
export constexpr double pi64 = std::numbers::pi_v<double>;
export constexpr float e32 = std::numbers::e_v<float>;
export constexpr double e64 = std::numbers::e_v<double>;

// integral types existence template class
export template<class I> class is_there
{
public:
	is_there(I value) :m_value{ value } {}
	virtual ~is_there() = default;
	template<I... Vals> constexpr bool in() { return ((m_value == Vals) || ...); }
private:
	I m_value;
};

// factorial calculator
export template<std::integral I> constexpr I factorial(I p)
{
	using T = decltype(p);
	constexpr auto zero{ static_cast<T>(0) };
	constexpr auto one{ static_cast<T>(1) };
	auto result{ zero };
	switch (p)
	{
	case zero:
		result = one;
		break;
	default:
		[&]()
			{
				auto r{ p };
				auto l{ p - one };
				while (l >= one)
				{
					r *= l;
					l--;
				}
				result = r;
			}();
		break;
	}
	return result;
}

// number of total combinations calculator
export template<std::integral I> constexpr I nCk(I n, I k) { return ((factorial(n) / (factorial(n - k) * factorial(k)))); }

/*****************************************************************************************
*
*                                   numerical traits
*
*****************************************************************************************/
export namespace integration
{
	// trapzoidal
	template<std::floating_point T, std::invocable<T> F> constexpr T trapzoidal(T x0, T xn, size_t n, F&& f)
	{
		auto h{ (xn - x0) / n };
		std::vector<T> fx(n + 1, T{ 0 });
		for (auto i : from(0).to(n))
			fx.data()[i] = std::invoke(std::forward<F>(f), x0 + (i * h));
		fx.data()[n] = std::invoke(std::forward<F>(f), xn);

		return (h / T{ 2 }) *
			(fx.data()[0] + T{ 2 } *std::reduce(std::execution::seq,
				std::ranges::next(std::ranges::begin(fx)),
				std::ranges::prev(std::ranges::prev(std::ranges::end(fx))),
				T{ 0 },
				std::plus<T>{}) + fx.data()[n]);
	}

	// simpson 1/3
	template<std::floating_point T, std::invocable<T> F> constexpr T simpson13(T x0, T xn, size_t n, F&& f)
	{
		auto h{ (xn - x0) / n };
		std::vector<T> fx(n + 1, T{ 0 });
		for (auto i : from(0).to(n))
			fx.data()[i] = std::invoke(std::forward<F>(f), x0 + (i * h));
		fx.data()[n] = std::invoke(std::forward<F>(f), xn);

		auto key{ true };
		auto s1{ T{0} }, s2{ T{0} };
		std::ranges::for_each(std::ranges::next(std::ranges::begin(fx)),
			std::ranges::prev(std::ranges::prev(std::ranges::end(fx))),
			[&key, &s1, &s2](auto& xi) { (key ? s1 += xi : s2 += xi); key = !key; });
		return (h / T{ 3 }) * (fx.data()[0] + T{ 4 } *s1 + T{ 2 } *s2 + fx.data()[n]);
	}
}
