export module statistics;

import <iterator>;
import <ranges>;
import <algorithm>;
import <numeric>;
import <execution>;
import concepts;

/*****************************************************************************************
*
*                                       statistics
*
*****************************************************************************************/
// fill ranges
export template<std::input_or_output_iterator It>
constexpr void fill(It beg, It end, std::iter_value_t<It> newval)
{
	std::ranges::for_each(beg, end, [&](auto& elem) { elem = newval; });
}

// add values in a range
export template<std::input_or_output_iterator It>
constexpr std::iter_value_t<It> add(It b, It e)
{
	using value_type = std::iter_value_t<It>;
	return std::reduce(std::execution::seq, b, e, (value_type)0, std::plus<value_type>());
}

// multiplies values in a range
export template<std::input_or_output_iterator It>
constexpr std::iter_value_t<It> multiply(It b, It e)
{
	using value_type = std::iter_value_t<It>;
	return std::reduce(std::execution::seq, b, e, (value_type)1, std::multiplies<value_type>());
}

// arithmetic mean calculator
export template<real_value_iterator Iter>
constexpr std::iter_value_t<Iter> arithmetic_mean(Iter b, Iter e)
{
	using value_type = std::iter_value_t<Iter>;
	auto mu{ (value_type)0 };
	auto range_size{ std::ranges::distance(b, e) };
	if (range_size == 1uz)
		mu = *b;
	else
		mu = add(b, e) / range_size;
	return mu;
}

// standard deviation calculator
export template<real_value_iterator Iter>
constexpr std::iter_value_t<Iter> standard_deviation(Iter b, Iter e, std::iter_value_t<Iter> mu)
{
	using value_type = std::iter_value_t<Iter>;
	auto sigma{ (value_type)0 };
	auto range_size{ std::ranges::distance(b, e) };
	if (range_size > 1uz)
		sigma = std::sqrt(std::transform_reduce(std::execution::seq,
			b,
			e,
			(value_type)0,
			std::plus<value_type>(),
			[mu](const auto& v) { return (v - mu) * (v - mu); })) / (range_size - 1uz);
	return sigma;
}

// normalizer
export template<real_value_iterator Iter> struct normalize
{
	Iter begin, end;
	normalize() :begin{}, end{}
	{
	}
	normalize(Iter b, Iter e) :begin{ b }, end{ e }
	{
	}
	constexpr void to(std::iter_value_t<Iter> newsum)
	{
		auto oldsum{ add(begin, end) };
		auto newsum_to_oldsum_ratio{ newsum / oldsum };
		std::ranges::for_each(begin, end, [&](auto& elem) { elem *= newsum_to_oldsum_ratio; });
	}
};
