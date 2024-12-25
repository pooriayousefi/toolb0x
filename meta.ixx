export module meta;

import <concepts>;
import <utility>;
import <functional>;
import concepts;

export namespace meta
{
	// meta data
	template<class T> struct data
	{
		using value_type = T;
		value_type value;
		data() :value{}
		{
		}
		data(const value_type& initial_value) :value{ initial_value }
		{
		}
		data(value_type&& initial_value) noexcept :value{ initial_value }
		{
		}
		template<std::invocable<value_type> F> constexpr decltype(auto) apply(F&& f)
		{
			return data<std::invoke_result_t<F, value_type>>(std::invoke(std::forward<F>(f), value));
		}
		template<procedure<value_type&> H, procedure<value_type&>... Fs> constexpr decltype(auto) pipe(H&& h, Fs&&... fs)
		{
			std::invoke(std::forward<H>(h), value);
			pipe(std::forward<Fs>(fs)...);
		}
		template<procedure<value_type&> F> constexpr decltype(auto) pipe(F&& f) { std::invoke(std::forward<F>(f), value); }
	};
}
