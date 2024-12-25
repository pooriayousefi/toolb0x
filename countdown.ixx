export module countdown;

import <concepts>;
import <chrono>;
import <thread>;
import <iostream>;

// countdown function
export template<std::integral I> constexpr void countdown(I nsec)
{
	std::cout << "\nT-" << nsec << ' ';
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (auto i{ static_cast<int64_t>(nsec) - static_cast<int64_t>(1) }; i >= static_cast<int64_t>(0); --i)
	{
		std::cout << i << ' ';
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
