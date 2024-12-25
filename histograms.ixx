export module histograms;

import <cstddef>;
import <functional>;
import <ranges>;
import <unordered_map>;
import <functional>;
import <utility>;
import <algorithm>;
import <string_view>;

namespace std
{
	// hash functor specialization for byte
	template<> struct hash<byte>
	{
		constexpr auto operator()(const byte& b) const -> size_t
		{
			return to_integer<size_t>(b);
		}
	};

	// equality functor specialization for byte
	template<> struct equal_to<byte>
	{
		constexpr auto operator()(const byte& lb, const byte& rb) const -> bool
		{
			return to_integer<size_t>(lb) == to_integer<size_t>(rb);
		}
	};

	// hash functor specialization for references/const references
	template<class T> struct hash<reference_wrapper<const T>>
	{
		constexpr auto operator()(const reference_wrapper<const T>& ref) const -> size_t
		{
			hash<T> hashval{};
			return hashval(ref.get());
		}
	};

	// equality functor specialization for references/const references
	template<class T> struct equal_to<reference_wrapper<const T>>
	{
		constexpr auto operator()(const reference_wrapper<const T>& lhs, const reference_wrapper<const T>& rhs) const -> bool
		{
			return lhs.get() == rhs.get();
		}
	};

	// less than functor specialization for references/const references
	template<class T> struct less<reference_wrapper<const T>>
	{
		constexpr auto operator()(const reference_wrapper<const T>& lhs, const reference_wrapper<const T>& rhs) const -> bool
		{
			return lhs.get() < rhs.get();
		}
	};

	// greater than functor specialization for references / const references
	template<class T> struct greater<reference_wrapper<const T>>
	{
		constexpr auto operator()(const reference_wrapper<const T>& lhs, const reference_wrapper<const T>& rhs) const -> bool
		{
			return lhs.get() > rhs.get();
		}
	};

	// less than equal to functor specialization for references/const references
	template<class T> struct less_equal<reference_wrapper<const T>>
	{
		constexpr auto operator()(const reference_wrapper<const T>& lhs, const reference_wrapper<const T>& rhs) const -> bool
		{
			return lhs.get() <= rhs.get();
		}
	};

	// greater than equal to functor specialization for references/const references
	template<class T> struct greater_equal<reference_wrapper<const T>>
	{
		constexpr auto operator()(const reference_wrapper<const T>& lhs, const reference_wrapper<const T>& rhs) const -> bool
		{
			return lhs.get() >= rhs.get();
		}
	};
}

// histogram of a range elements
export template<std::ranges::range Rng> constexpr decltype(auto) histogram(const Rng& rng)
{
	std::unordered_map<
		std::reference_wrapper<const std::ranges::range_value_t<Rng>>,
		size_t,
		std::hash<std::reference_wrapper<const std::ranges::range_value_t<Rng>>>,
		std::equal_to<std::reference_wrapper<const std::ranges::range_value_t<Rng>>>,
		std::allocator<std::pair<const std::reference_wrapper<const std::ranges::range_value_t<Rng>>, size_t>>
	> h{};
	for (const auto& elem : rng)
		h[std::cref(elem)]++;
	return h;
}

export template<std::input_or_output_iterator It> constexpr decltype(auto) histogram(It beg, It end)
{
	std::unordered_map<
		std::reference_wrapper<const std::iter_value_t<It>>,
		size_t,
		std::hash<std::reference_wrapper<const std::iter_value_t<It>>>,
		std::equal_to<std::reference_wrapper<const std::iter_value_t<It>>>,
		std::allocator<std::pair<const std::reference_wrapper<const std::iter_value_t<It>>, size_t>>
	> h{};
	std::ranges::for_each(beg, end, [&](const auto& elem) { h[std::cref(elem)]++; });
	return h;
}

export template<class Enc, class EncTraits = std::char_traits<Enc>> constexpr decltype(auto) frequencies(
	std::basic_string_view<Enc, EncTraits> query,
	std::basic_string_view<Enc, EncTraits> delimiters
)
{
	std::unordered_map<
		std::basic_string_view<Enc, EncTraits>,
		size_t,
		std::hash<std::basic_string_view<Enc, EncTraits>>,
		std::equal_to<std::basic_string_view<Enc, EncTraits>>,
		std::allocator<std::pair<const std::basic_string_view<Enc, EncTraits>, size_t>>
	> freqs{};

	// skip delimiters at begining
	auto last_pos = query.find_first_not_of(delimiters, 0);

	// find first non-delimiter
	auto pos = query.find_first_of(delimiters, last_pos);

	while (pos != std::basic_string_view<Enc, EncTraits>::npos || last_pos != std::basic_string_view<Enc, EncTraits>::npos)
	{
		// find a token, add it to the vector
		freqs[query.substr(last_pos, pos - last_pos)]++;

		// skip delimiters
		last_pos = query.find_first_not_of(delimiters, pos);

		// find next non-delimiter
		pos = query.find_first_of(delimiters, last_pos);
	}

	return freqs;
}