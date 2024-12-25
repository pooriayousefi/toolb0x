export module concepts;

import <type_traits>;
import <concepts>;
import <ranges>;

/*****************************************************************************************
*
*                                       concepts
*
*****************************************************************************************/
export template<typename T> concept default_constructible = std::is_default_constructible_v<T>;
export template<typename T> concept arithmetic = std::floating_point<T> || std::integral<T>;
export template<typename It> concept not_input_or_output_iterator = !std::input_or_output_iterator<It>;
export template<typename T> concept not_range = !std::ranges::range<T>;
export template<typename It> concept real_value_iterator = std::input_or_output_iterator<It> && std::floating_point<std::iter_value_t<It>>;
export template<typename Rng> concept real_value_range = std::ranges::range<Rng> && std::floating_point<std::ranges::range_value_t<Rng>>;
export template<typename T, typename... Ts> concept either_or = (std::is_same_v<T, Ts> || ...);
export template<typename F, typename... Args> concept procedure = std::invocable<F, Args...>&& std::is_void_v<std::invoke_result_t<F, Args...>>;
export template<typename F, typename... Args> concept real_value_invocable = std::invocable<F, Args...>&& std::floating_point<std::invoke_result_t<F, Args...>>;
