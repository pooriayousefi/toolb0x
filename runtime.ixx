export module runtime;

import <type_traits>;
import <chrono>;
import <functional>;
import <utility>;

// runtime calculator
export template<typename F, typename... Args> constexpr decltype(auto) runtime(F&& f, Args&&... args)
{
	if constexpr (std::is_void_v<std::invoke_result_t<F, Args...>>)
	{
		auto ti{ std::chrono::high_resolution_clock::now() };
		std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
		auto tf{ std::chrono::high_resolution_clock::now() };
		return std::chrono::duration<double>(tf - ti).count();
	}
	else if constexpr (!std::is_void_v<std::invoke_result_t<F, Args...>>)
	{
		auto ti{ std::chrono::high_resolution_clock::now() };
		auto retval{ std::invoke(std::forward<F>(f), std::forward<Args>(args)...) };
		auto tf{ std::chrono::high_resolution_clock::now() };
		return std::make_pair(std::move(retval), std::chrono::duration<double>(tf - ti).count());
	}
}
