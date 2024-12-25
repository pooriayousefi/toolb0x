export module time_frames;

import <concepts>;
import <valarray>;
import <ranges>;

export template<std::integral I> struct time_frames
{
	double initial_time, final_time;
	I number_of_time_steps;
	time_frames(double ti, double tf, I nts) :
		initial_time{ ti },
		final_time{ tf },
		number_of_time_steps{ nts }
	{
	}
	inline std::valarray<double> generate()
	{
		std::valarray<double> t(initial_time, number_of_time_steps);
		auto dt{ (final_time - initial_time) / (number_of_time_steps - 1.0) };		
		for (auto i : std::ranges::views::iota((size_t)1, (size_t)number_of_time_steps))
			t[i] = initial_time + (i * dt);
		return t;
	}
	inline std::valarray<double> operator()() { return generate(); }
};
