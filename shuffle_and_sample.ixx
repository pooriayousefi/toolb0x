export module shuffle_and_sample;

import <iterator>;
import <vector>;
import <random>;
import <ranges>;
import <algorithm>;

// shuffle a range and sample it n times
export template<std::input_or_output_iterator It>
constexpr std::vector<std::iter_value_t<It>> do_n_times_shuffle_and_sample(It beg, It end, size_t n)
{
	std::random_device rd{};
	std::mt19937_64 random_number_generator{ rd() };
	std::vector<std::iter_value_t<It>> samples(n);
	auto it{ std::ranges::begin(samples) };
	while (it != std::ranges::end(samples))
	{
		std::ranges::shuffle(beg, end, random_number_generator);
		std::ranges::sample(beg, end, it, 1, random_number_generator);
		it++;
	}
	return samples;
}
