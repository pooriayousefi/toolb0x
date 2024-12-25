export module conversions;

import <concepts>;
import <numbers>;
import <vector>;
import <cstdint>;
import <ranges>;
import <algorithm>;

/*****************************************************************************************
*
*                                      conversions
*
*****************************************************************************************/
export namespace convert
{
	template<std::floating_point T> constexpr T degrees_to_radians(T x) { return x * std::numbers::pi_v<T> / (T)180; }
	template<std::floating_point T> constexpr T radians_to_degrees(T x) { return x * (T)180 / std::numbers::pi_v<T>; }
	template<std::floating_point T> constexpr T Celsius_to_Fahrenheit(T x) { return (x * (T)9 / (T)5) + (T)32; }
	template<std::floating_point T> constexpr T Fahrenheit_to_Celsius(T x) { return (x - (T)32) * (T)5 / (T)9; }
	template<std::integral I> std::vector<uint8_t> decimal_to_binary(I n)
	{
		std::vector<uint8_t> binval{};
		binval.reserve(64);
		if (n == (I)0)
			binval.emplace_back('0');
		else
		{
			while (n != (I)0)
			{
				binval.emplace_back(static_cast<uint8_t>(n % (I)2));
				n /= (I)2;
			}
			binval.emplace_back('b');
			binval.emplace_back('0');
			std::ranges::reverse(std::ranges::begin(binval), std::ranges::end(binval));
		}
		return binval;
	}
	template<std::integral I> std::vector<char> decimal_to_hexadecimal(I n)
	{
		std::vector<char> hexval{};
		hexval.reserve(64);
		if (n == (I)0)
		{
			hexval.emplace_back('0');
			hexval.emplace_back('x');
			hexval.emplace_back('0');
		}
		else
		{
			while (n != (I)0)
			{
				auto rem{ n % (I)16 };
				switch (rem)
				{
				case (I)10:
					hexval.emplace_back('A');
					break;
				case (I)11:
					hexval.emplace_back('B');
					break;
				case (I)12:
					hexval.emplace_back('C');
					break;
				case (I)13:
					hexval.emplace_back('D');
					break;
				case (I)14:
					hexval.emplace_back('E');
					break;
				case (I)15:
					hexval.emplace_back('F');
					break;
				default:
					break;
				}
				n /= (I)16;
			}
			hexval.emplace_back('x');
			hexval.emplace_back('0');
			std::ranges::reverse(std::ranges::begin(hexval), std::ranges::end(hexval));
		}
		return hexval;
	}
}
