export module from_to;

import <concepts>;
import <ranges>;

/*****************************************************************************************
*
*                                     range indexing
*
*****************************************************************************************/
export template<std::integral I> struct from
{
	I initial_value;
	from() :initial_value(0) {}
	from(I inival) :initial_value(inival) {}
	template<std::integral J> constexpr decltype(auto) to(J exclusive_final_value)
	{
		return std::ranges::views::iota(static_cast<J>(initial_value), exclusive_final_value);
	}
};
