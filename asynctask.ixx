export module asynctask;

import <thread>;
import <coroutine>;
import <variant>;
import <exception>;
import <utility>;
import <semaphore>;
import <memory>;
import <cassert>;

export namespace asynchronous
{
	// awaitable task data structure
	template<class T> struct task
	{
		struct promise_type
		{
			std::variant<std::monostate, T, std::exception_ptr> result;
			std::coroutine_handle<> continuation;
			constexpr decltype(auto) get_return_object() noexcept { return task{ *this }; }
			constexpr void return_value(T value) { result.template emplace<1>(std::move(value)); }
			constexpr void unhandled_exception() noexcept { result.template emplace<2>(std::current_exception()); }
			constexpr decltype(auto) initial_suspend() { return std::suspend_always{}; }
			struct awaitable
			{
				constexpr bool await_ready() noexcept { return false; }
				constexpr decltype(auto) await_suspend(std::coroutine_handle<promise_type> h) noexcept
				{
					return h.promise().continuation;
				}
				constexpr void await_resume() noexcept {}
			};
			constexpr decltype(auto) final_suspend() noexcept { return awaitable{}; }
		};
		std::coroutine_handle<promise_type> handle;
		explicit task(promise_type& p) noexcept :handle{ std::coroutine_handle<promise_type>::from_promise(p) } {}
		task(task&& t) noexcept :handle{ t.handle } {}
		~task() { if (handle) handle.destroy(); }
		constexpr bool await_ready() { return false; }
		constexpr decltype(auto) await_suspend(std::coroutine_handle<> c)
		{
			handle.promise().continuation = c;
			return handle;
		}
		constexpr T await_resume()
		{
			auto& result = handle.promise().result;
			if (result.index() == 1)
				return std::get<1>(std::move(result));
			else
				std::rethrow_exception(std::get<2>(std::move(result)));
		}
	};
	template<> struct task<void>
	{
		struct promise_type
		{
			std::exception_ptr e;
			std::coroutine_handle<> continuation;
			inline decltype(auto) get_return_object() noexcept { return task{ *this }; }
			constexpr void return_void() {}
			inline void unhandled_exception() noexcept { e = std::current_exception(); }
			constexpr decltype(auto) initial_suspend() { return std::suspend_always{}; }
			struct awaitable
			{
				constexpr bool await_ready() noexcept { return false; }
				inline decltype(auto) await_suspend(std::coroutine_handle<promise_type> h) noexcept
				{
					return h.promise().continuation;
				}
				constexpr void await_resume() noexcept {}
			};
			constexpr decltype(auto) final_suspend() noexcept { return awaitable{}; }
		};
		std::coroutine_handle<promise_type> handle;
		explicit task(promise_type& p) noexcept :handle{ std::coroutine_handle<promise_type>::from_promise(p) } {}
		task(task&& t) noexcept :handle{ t.handle } {}
		~task() { if (handle) handle.destroy(); }
		constexpr bool await_ready() { return false; }
		inline decltype(auto) await_suspend(std::coroutine_handle<> c)
		{
			handle.promise().continuation = c;
			return handle;
		}
		inline void await_resume()
		{
			if (handle.promise().e)
				std::rethrow_exception(handle.promise().e);
		}
	};

	// result data structure
	template<class T> using result_type = decltype(std::declval<T&>().await_resume());

	/*
	The forced chaining of coroutines has the interesting effect that we finally get to the
	main() function of the program, which the C++ standard says is not allowed to be
	a coroutine. This needs to be addressed somehow, and the proposed solution is to
	provide at least one function that synchronously waits on the asynchronous chains
	to complete. For example, the CppCoro library includes the function sync_wait(),
	which has this effect of breaking the chain of coroutines, which makes it possible
	for an ordinary function to use coroutines.
	*/
	// implementing sync_wait_task helper data structure
	template<class T> struct sync_wait_task
	{
		struct promise_type
		{
			T* value{ nullptr };
			std::exception_ptr error{ nullptr };
			std::binary_semaphore sema4{ 0 };
			inline sync_wait_task get_return_object() noexcept { return sync_wait_task{ *this }; }
			constexpr void unhandled_exception() noexcept { error = std::current_exception(); }
			constexpr decltype(auto) yield_value(T&& x) noexcept
			{
				value = std::addressof(x);
				return final_suspend();
			}
			constexpr decltype(auto) initial_suspend() noexcept { return std::suspend_always{}; }
			struct awaitable
			{
				constexpr bool await_ready() noexcept { return false; }
				constexpr void await_suspend(std::coroutine_handle<promise_type> h) noexcept { h.promise().sema4.release(); }
				constexpr void await_resume() noexcept {}
			};
			constexpr decltype(auto) final_suspend() noexcept { return awaitable{}; }
			constexpr void return_void() noexcept { assert(false); }
		};
		std::coroutine_handle<promise_type> handle;
		explicit sync_wait_task(promise_type& p) noexcept :handle{ std::coroutine_handle<promise_type>::from_promise(p) } {}
		sync_wait_task(sync_wait_task&& t) noexcept :handle{ t.handle } {}
		~sync_wait_task() { if (handle) handle.destroy(); }
		inline T&& get()
		{
			auto& p = handle.promise();
			handle.resume();
			p.sema4.acquire();
			if (p.error)
				std::rethrow_exception(p.error);
			return static_cast<T&&>(*p.value);
		}
	};

	// implementing sync_wait()
	template<class T> result_type<T> sync_wait(T&& task)
	{
		if constexpr (std::is_void_v<result_type<T>>)
		{
			struct empty_type {};
			auto coro = [&]() -> sync_wait_task<empty_type>
				{
					co_await std::forward<T>(task);
					co_yield empty_type{};
					assert(false);
				};
			coro().get();
		}
		else
		{
			auto coro = [&]() -> sync_wait_task<result_type<T>>
				{
					co_yield co_await std::forward<T>(task);
					assert(false);
				};
			return coro().get();
		}
	}
}
