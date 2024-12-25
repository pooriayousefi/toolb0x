export module wait_for;

import <chrono>;
import <thread>;

template<typename T> concept arithmetic = std::floating_point<T> || std::integral<T>;

// wait for some times in different time units
export template<arithmetic T> class wait_for
{
public:
	wait_for() = delete;
	wait_for(T value) :m_value{ value } {}
	inline void nanoseconds() { std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1'000'000'000>>(m_value)); }
	inline void microseconds() { std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1'000'000>>(m_value)); }
	inline void milliseconds() { std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1'000>>(m_value)); }
	inline void seconds() { std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1>>(m_value)); }
	inline void minutes() { std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<60>>(m_value)); }
	inline void hours() { std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<3'600>>(m_value)); }
	inline void days() { std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<86'400>>(m_value)); }
private:
	T m_value;
};