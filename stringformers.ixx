export module stringformers;

import <cctype>;
import <string>;
import <string_view>;
import <ranges>;
import <algorithm>;

// to lowercase function
export template<class Enc, class EncTraits = std::char_traits<Enc>, class EncAlloc = std::allocator<Enc>>
constexpr decltype(auto) to_lowercase(const std::basic_string<Enc, EncTraits, EncAlloc>& word)
{
	std::basic_string<Enc, EncTraits, EncAlloc> lowercased_word{};
	lowercased_word.resize(std::ranges::size(word));
	std::ranges::transform(std::ranges::cbegin(word), std::ranges::cend(word),
		std::ranges::begin(lowercased_word), [](const auto& c) { return std::tolower(c); });
	return lowercased_word;
}
export template<class Enc, class EncTraits = std::char_traits<Enc>, class EncAlloc = std::allocator<Enc>>
constexpr decltype(auto) to_lowercase(std::basic_string_view<Enc, EncTraits> word_view)
{
	std::basic_string<Enc, EncTraits, EncAlloc> lowercased_word{};
	lowercased_word.resize(std::ranges::size(word_view));
	std::ranges::transform(std::ranges::cbegin(word_view), std::ranges::cend(word_view),
		std::ranges::begin(lowercased_word), [](const auto& c) { return std::tolower(c); });
	return lowercased_word;
}

// to uppercase function
export template<class Enc, class EncTraits = std::char_traits<Enc>, class EncAlloc = std::allocator<Enc>>
constexpr decltype(auto) to_uppercase(const std::basic_string<Enc, EncTraits, EncAlloc>& word)
{
	std::basic_string<Enc, EncTraits, EncAlloc> uppercased_word{};
	uppercased_word.resize(std::ranges::size(word));
	std::ranges::transform(std::ranges::cbegin(word), std::ranges::cend(word),
		std::ranges::begin(uppercased_word), [](const auto& c) { return std::toupper(c); });
	return uppercased_word;
}
export template<class Enc, class EncTraits = std::char_traits<Enc>, class EncAlloc = std::allocator<Enc>>
constexpr decltype(auto) to_uppercase(std::basic_string_view<Enc, EncTraits> word_view)
{
	std::basic_string<Enc, EncTraits, EncAlloc> uppercased_word{};
	uppercased_word.resize(std::ranges::size(word_view));
	std::ranges::transform(std::ranges::cbegin(word_view), std::ranges::cend(word_view),
		std::ranges::begin(uppercased_word), [](const auto& c) { return std::toupper(c); });
	return uppercased_word;
}
