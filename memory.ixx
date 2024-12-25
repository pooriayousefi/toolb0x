export module memory;

import <cstddef>;
import <memory>;
import <iterator>;
import <ranges>;
import <algorithm>;
import <vector>;
import concepts;
import file_streams;
import misc;

// decimal address reader
export template<not_input_or_output_iterator T>
constexpr decltype(auto) decimal_memory_address(T& obj) { return reinterpret_cast<uintptr_t>(&obj); }
export template<not_input_or_output_iterator T>
constexpr decltype(auto) decimal_memory_address(T* obj) { return reinterpret_cast<uintptr_t>(&(*obj)); }
export template<std::input_or_output_iterator It>
constexpr decltype(auto) decimal_memory_address(It it) { return reinterpret_cast<uintptr_t>(&(*it)); }
export template<std::input_or_output_iterator It>
constexpr decltype(auto) decimal_memory_address(native_narrow_encoded_output_file& of, It beg, It end)
{
	of.stream
		<< " value\t\taddress\t\taddress difference with next item\n"
		<< " -----\t\t-------\t\t---------------------------------\n\n";
	for (auto it{ beg }; it != std::ranges::prev(end); it = std::ranges::next(it))
		of.stream
		<< ' ' << *it << "\t\t" << decimal_memory_address(it)
		<< "\t\t" << decimal_memory_address(std::ranges::next(it)) - decimal_memory_address(it)
		<< '\n';
	of.stream
		<< ' ' << *(std::ranges::prev(end)) << "\t\t" << decimal_memory_address(std::ranges::prev(end))
		<< std::endl;
}
export template<std::input_or_output_iterator It>
constexpr decltype(auto) decimal_memory_address(native_wide_encoded_output_file& of, It beg, It end)
{
	of.stream
		<< L" value\t\taddress\t\taddress difference with next item\n"
		<< L" -----\t\t-------\t\t---------------------------------\n\n";
	for (auto it{ beg }; it != std::ranges::prev(end); it = std::ranges::next(it))
		of.stream
		<< L' ' << *it << L"\t\t" << decimal_memory_address(it)
		<< L"\t\t" << decimal_memory_address(std::ranges::next(it)) - decimal_memory_address(it)
		<< L'\n';
	of.stream
		<< L' ' << *(std::ranges::prev(end)) << L"\t\t" << decimal_memory_address(std::ranges::prev(end))
		<< std::endl;
}

//  object pool
export template<default_constructible T, size_t ChunkSize> class object_pool
{
public:
	using value_type = T;
	using type = object_pool<value_type, ChunkSize>;
	using data_structure = table<value_type>;
private:
	alignas(value_type) data_structure m_object_pool;
	size_t m_counter;
	constexpr void allocate_chunk() { m_object_pool.emplace_back(std::vector<value_type>(ChunkSize, value_type())); }
public:
	object_pool() :
		m_object_pool{},
		m_counter{ static_cast<size_t>(0) }
	{
		m_object_pool.reserve(static_cast<size_t>(2) * ChunkSize);
		allocate_chunk();
	}
	object_pool(const type&) = delete;
	object_pool(type&&) noexcept = delete;
	virtual ~object_pool() = default;
	auto operator=(const type&)->type & = delete;
	auto operator=(type&&) noexcept -> type & = delete;
	bool operator==(const object_pool& other) const = delete;
	constexpr value_type* get_object()
	{
		if (m_counter == ChunkSize)
		{
			allocate_chunk();
			m_counter = static_cast<size_t>(0);
		}
		return &m_object_pool.back()[m_counter++];
	}
};

// single-thread fixed-size memory manager (arena)
export template<size_t N> class arena
{
private:
	static constexpr inline const size_t m_alignment = alignof(std::max_align_t);
	static inline size_t align_up(size_t n) noexcept { return (n + (m_alignment - static_cast<size_t>(1))) & ~(m_alignment - static_cast<size_t>(1)); }
	constexpr bool pointer_in_buffer(const std::byte* p) const noexcept { return m_buffer <= p && p <= m_buffer + N; }
	alignas(m_alignment) std::byte m_buffer[N];
	std::byte* m_ptr{};
public:
	arena() noexcept : m_ptr(m_buffer) {}
	arena(const arena&) = delete;
	arena& operator=(const arena&) = delete;
	constexpr bool reset() noexcept { m_ptr = m_buffer; }
	static constexpr size_t size() noexcept { return N; }
	constexpr size_t used() const noexcept { return static_cast<size_t>(m_ptr - m_buffer); }
	constexpr std::byte* allocate(size_t n)
	{
		const auto aligned_n = align_up(n);
		const auto available_bytes = static_cast<decltype(aligned_n)>(m_buffer + N - m_ptr);
		if (available_bytes >= aligned_n)
		{
			std::byte* r = m_ptr;
			m_ptr += aligned_n;
			return r;
		}
		return static_cast<std::byte*>(::operator new(n));
	}
	constexpr void deallocate(std::byte* p, size_t n) noexcept
	{
		if (pointer_in_buffer(p))
		{
			n = align_up(n);
			if (p + n == m_ptr)
			{
				m_ptr = p;
			}
		}
		else
		{
			::operator delete(p);
		}
	}
};
