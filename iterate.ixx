export module iterate;

import <concepts>;
import <iterator>;
import <functional>;
import <utility>;
import <ranges>;
import <algorithm>;

// iterate through a range step sized
template<std::input_or_output_iterator It, std::invocable<std::iter_value_t<It>&> F>
constexpr auto iterate(It begin, size_t n, size_t step_size, F&& f) -> void
{
	auto c = static_cast<size_t>(0);
	auto it = begin;
	do
	{
		std::invoke(std::forward<F>(f), *it);
		c++;
	} while (c < n && [&]() { it = std::ranges::next(it, step_size); return true; }());
}