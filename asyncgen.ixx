export module asyncgen;

import <thread>;
import <coroutine>;
import <iterator>;
import <exception>;

export namespace asynchronous
{
	// resumable generator data structure
	template<class T> struct generator
	{
		struct promise_type
		{
			T current_value;
			constexpr decltype(auto) initial_suspend() { return std::suspend_always{}; }
			constexpr decltype(auto) final_suspend() noexcept { return std::suspend_always{}; }
			constexpr decltype(auto) get_return_object() { return generator{ std::coroutine_handle<promise_type>::from_promise(*this) }; }
			constexpr decltype(auto) return_void() { return std::suspend_never{}; }
			constexpr decltype(auto) yield_value(T&& value) noexcept { current_value = value; return std::suspend_always{}; }
			inline void unhandled_exception() { std::terminate(); }
		};
		struct sentinel {};
		struct iterator
		{
			using iterator_category = std::input_iterator_tag;
			using value_type = T;
			using difference_type = ptrdiff_t;
			using pointer = T*;
			using reference = T&;
			using const_reference = const T&;
			std::coroutine_handle<promise_type> handle;
			explicit iterator(std::coroutine_handle<promise_type>& h) :handle{ h } {}
			constexpr iterator& operator++()
			{
				handle.resume();
				return *this;
			}
			constexpr void operator++(int) { (void)operator++(); }
			constexpr reference operator*() { return handle.promise().current_value; }
			constexpr pointer operator->() { return std::addressof(operator*()); }
			constexpr const_reference operator*() const { return handle.promise().current_value; }
			constexpr pointer operator->() const { return std::addressof(operator*()); }
			constexpr bool operator==(sentinel) { return handle.done(); }
			constexpr bool operator==(sentinel) const { return handle.done(); }
		};
		std::coroutine_handle<promise_type> handle;
		explicit generator(std::coroutine_handle<promise_type> h) :handle{ h } {}
		~generator() { if (handle) handle.destroy(); }
		generator(const generator&) = delete;
		generator(generator&& other) noexcept :handle(other.handle) { other.handle = nullptr; }
		generator& operator=(const generator&) = delete;
		generator& operator=(generator&& other) noexcept { handle = other.handle; other.handle = nullptr; return *this; }
		constexpr T get_value() { return handle.promise().current_value; }
		constexpr bool next() { handle.resume(); return !handle.done(); }
		constexpr bool resume() { handle.resume(); return !handle.done(); }
		constexpr decltype(auto) begin()
		{
			handle.resume();
			return iterator{ handle };
		}
		constexpr decltype(auto) end() { return sentinel{}; }
	};
}
