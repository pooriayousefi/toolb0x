export module tokenize;

import <functional>;
import <string>;
import <string_view>;
import <vector>;
import <unordered_set>;

// string tokenization various implementations
export template<class Enc, class EncTraits = std::char_traits<Enc>, class EncAlloc = std::allocator<Enc>>
constexpr void tokenize(
	const std::basic_string<Enc, EncTraits, EncAlloc>& sentence,
	const std::basic_string<Enc, EncTraits, EncAlloc>& delimiters,
	std::vector<std::basic_string<Enc, EncTraits, EncAlloc>>& tokens
)
{
	// skip delimiters at begining
	auto last_pos = sentence.find_first_not_of(delimiters, 0);

	// find first non-delimiter
	auto pos = sentence.find_first_of(delimiters, last_pos);

	while (pos != std::basic_string<Enc, EncTraits, EncAlloc>::npos || last_pos != std::basic_string<Enc, EncTraits, EncAlloc>::npos)
	{
		// find a token, add it to the vector
		tokens.emplace_back(sentence.substr(last_pos, pos - last_pos));

		// skip delimiters
		last_pos = sentence.find_first_not_of(delimiters, pos);

		// find next non-delimiter
		pos = sentence.find_first_of(delimiters, last_pos);
	}
}

export template<class Enc, class EncTraits = std::char_traits<Enc>>
constexpr void tokenize(
	std::basic_string_view<Enc, EncTraits> query,
	std::basic_string_view<Enc, EncTraits> delimiters,
	std::unordered_multiset<
	std::basic_string_view<Enc, EncTraits>,
	std::hash<std::basic_string_view<Enc, EncTraits>>,
	std::equal_to<std::basic_string_view<Enc, EncTraits>>,
	std::allocator<std::basic_string_view<Enc, EncTraits>>
	>& tokens
)
{
	// skip delimiters at begining
	auto last_pos = query.find_first_not_of(delimiters, 0);

	// find first non-delimiter
	auto pos = query.find_first_of(delimiters, last_pos);

	while (pos != std::basic_string_view<Enc, EncTraits>::npos || last_pos != std::basic_string_view<Enc, EncTraits>::npos)
	{
		// find a token, add it to the vector
		tokens.emplace(query.substr(last_pos, pos - last_pos));

		// skip delimiters
		last_pos = query.find_first_not_of(delimiters, pos);

		// find next non-delimiter
		pos = query.find_first_of(delimiters, last_pos);
	}
}

export template<class Enc, class EncTraits = std::char_traits<Enc>>
constexpr std::vector<std::basic_string_view<Enc, EncTraits>> tokenize(
	std::basic_string_view<Enc, EncTraits> query,
	std::basic_string_view<Enc, EncTraits> delimiters
)
{
	std::vector<std::basic_string_view<Enc, EncTraits>> tokens{};
	tokens.reserve(query.size());

	// skip delimiters at begining
	auto last_pos = query.find_first_not_of(delimiters, 0);

	// find first non-delimiter
	auto pos = query.find_first_of(delimiters, last_pos);

	while (pos != std::basic_string_view<Enc, EncTraits>::npos || last_pos != std::basic_string_view<Enc, EncTraits>::npos)
	{
		// find a token, add it to the vector
		tokens.emplace_back(query.substr(last_pos, pos - last_pos));

		// skip delimiters
		last_pos = query.find_first_not_of(delimiters, pos);

		// find next non-delimiter
		pos = query.find_first_of(delimiters, last_pos);
	}

	return tokens;
}
