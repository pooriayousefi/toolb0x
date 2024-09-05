
#ifndef __TOOLB0X_DEVELOPED_BY_POORIA_YOUSEFI_AT_8_2_2024_H_
#define __TOOLB0X_DEVELOPED_BY_POORIA_YOUSEFI_AT_8_2_2024_H_

// utilities
#include<stdexcept>
#include<exception>
#include<type_traits>
#include<concepts>
#include<typeinfo>
#include<typeindex>
#include<utility>
#include<functional>
#include<execution>
#include<algorithm>
#include<ranges>
#include<cassert>

// I/Os
#include<filesystem>
#include<iostream>
#include<fstream>
#include<ios>
#include<syncstream>
#include<spanstream>

// numerics
#include<cstddef>
#include<cstdint>
#include<cmath>
#include<numeric>
#include<numbers>
#include<limits>
#include<random>

// data and times
#include<chrono>

// memory managements
#include<memory>
#include<memory_resource>

// data structures
#include<vector>
#include<valarray>
#include<string>
#include<string_view>
#include<cstring>
#include<variant>
#include<optional>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<bitset>
#include<forward_list>
#include<list>
#include<array>
#include<stack>
#include<queue>
#include<deque>
#include<tuple>
#include<span>

// concurrency
#include<atomic>
#include<thread>
#include<future>
#include<coroutine>
#include<semaphore>
#include<condition_variable>
#include<barrier>
#include<latch>
#include<mutex>

// standard namespace extension
namespace std
{
	// hash functor specialization for byte
	template<> struct hash<byte>
	{
		constexpr auto operator()(const byte& b) const -> size_t
		{
			return to_integer<size_t>(b);
		}
	};

	// equality functor specialization for byte
	template<> struct equal_to<byte>
	{
		constexpr auto operator()(const byte& lb, const byte& rb) const -> bool
		{
			return to_integer<size_t>(lb) == to_integer<size_t>(rb);
		}
	};

	// hash functor specialization for references/const references
	template<typename T> struct hash<reference_wrapper<T>>
	{
		constexpr auto operator()(const reference_wrapper<T>& ref) const -> size_t
		{
			hash<T> hashval{};
			return hashval(ref.get());
		}
		constexpr auto operator()(reference_wrapper<T>&& ref) const noexcept -> size_t
		{
			hash<T> hashval{};
			return hashval(ref.get());
		}
	};

	// equality functor specialization for references/const references
	template<typename T> struct equal_to<reference_wrapper<T>>
	{
		constexpr auto operator()(const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs) const -> bool
		{
			return lhs.get() == rhs.get();
		}
		constexpr auto operator()(reference_wrapper<T>&& lhs, reference_wrapper<T>&& rhs) const noexcept -> bool
		{
			return lhs.get() == rhs.get();
		}
	};
}

// anonymous namespace
namespace
{
	// t++lb0x namespace
	namespace tbx
	{
		/*****************************************************************************************
		*
		*                                     type aliases
		*
		*****************************************************************************************/
		template<typename T> struct Literal
		{
			using type = T;
			template<std::convertible_to<T> X> static constexpr T value(X x) noexcept { return static_cast<T>(x); }
			static constexpr T minimum_value() noexcept { return std::numeric_limits<T>::min(); }
			static constexpr T maximum_value() noexcept { return std::numeric_limits<T>::max(); }
		};
		namespace signed_integer
		{
			using _8bit = Literal<int8_t>;
			using _16bit = Literal<int16_t>;
			using _32bit = Literal<int32_t>;
			using _64bit = Literal<int64_t>;
		}
		namespace unsigned_integer
		{
			using _8bit = Literal<uint8_t>;
			using _16bit = Literal<uint16_t>;
			using _32bit = Literal<uint32_t>;
			using _64bit = Literal<uint64_t>;
		}
		namespace real
		{
			using _32bit = Literal<float>;
			using _64bit = Literal<double>;
			using single_precision = Literal<float>;
			using double_precision = Literal<double>;
		}
		using Size = Literal<size_t>;
		
		template<typename T, typename AllocT = std::allocator<T>, typename AllocVecT = std::allocator<std::vector<T, AllocT>>>
		using table = std::vector<std::vector<T, AllocT>, AllocVecT>;
		template<typename T> using dynamic_array = std::valarray<T>;
		template<typename T> using sequence = std::valarray<T>;
		template<typename T, typename EncAlloc = std::allocator<T>> using linked_list = std::forward_list<T, EncAlloc>;
		template<typename T, typename EncAlloc = std::allocator<T>> using double_linked_list = std::list<T, EncAlloc>;
		template<typename K,
			typename Hasher = std::hash<K>,
			typename KEq = std::equal_to<K>,
			typename EncAlloc = std::allocator<K>>
			using hash_set = std::unordered_set<K, Hasher, KEq, EncAlloc>;
		template<typename K,
			typename V,
			typename Hasher = std::hash<K>,
			typename KEq = std::equal_to<K>,
			typename EncAlloc = std::allocator<std::pair<const K, V>>>
		using hash_map = std::unordered_map<K, V, Hasher, KEq, EncAlloc>;
		template<typename K,
			typename Hasher = std::hash<K>,
			typename KEq = std::equal_to<K>,
			typename EncAlloc = std::allocator<K>>
			using hash_multiset = std::unordered_multiset<K, Hasher, KEq, EncAlloc>;
		template<typename K,
			typename V,
			typename Hasher = std::hash<K>,
			typename KEq = std::equal_to<K>,
			typename EncAlloc = std::allocator<std::pair<const K, V>>>
		using hash_multimap = std::unordered_multimap<K, V, Hasher, KEq, EncAlloc>;

		using continuation_type = std::coroutine_handle<>;

		using high_resolution_clock = std::chrono::high_resolution_clock;
		using system_clock = std::chrono::system_clock;
		using steady_clock = std::chrono::steady_clock;
		template<typename Rep = real::double_precision::type, typename Period = std::ratio<1>> using duration = std::chrono::duration<Rep, Period>;

		template<typename T> using iterator_value_type = std::iter_value_t<T>;
		template<typename Rng> using range_value_type = std::ranges::range_value_t<Rng>;

		template<typename T, typename Del = std::default_delete<T>> constexpr decltype(auto) mkunqptr(T&& obj) { return std::make_unique<T, Del>(obj); }
		template<typename T> constexpr decltype(auto) mkshrptr(T&& obj) { return std::make_shared<T>(obj); }

		namespace native
		{
			using narrow_encoding = char;
			namespace narrow_encoded
			{
				using character = char;
				using string = std::string;
				using string_view = std::string_view;
				using input_stream = std::istream;
				using input_file_stream = std::ifstream;
				using output_stream = std::ostream;
				using output_file_stream = std::ofstream;
			}
			using wide_encoding = wchar_t;
			namespace wide_encoded
			{
				using character = wchar_t;
				using string = std::wstring;
				using string_view = std::wstring_view;
				using input_stream = std::wistream;
				using input_file_stream = std::wifstream;
				using output_stream = std::wostream;
				using output_file_stream = std::wofstream;
			}
		}
		using utf8_encoding = char8_t;
		namespace utf8
		{
			using character = char8_t;
			using string = std::u8string;
			using string_view = std::u8string_view;
			using input_stream = std::basic_istream<char8_t>;
			using input_file_stream = std::basic_ifstream<char8_t>;
			using output_stream = std::basic_ostream<char8_t>;
			using output_file_stream = std::basic_ofstream<char8_t>;
		}

		using binary_string = std::basic_string<std::byte>;
		using binary_string_view = std::basic_string_view<std::byte>;
		using binary_input_stream = std::basic_istream<std::byte>;
		using binary_input_file_stream = std::basic_ifstream<std::byte>;
		using binary_output_stream = std::basic_ostream<std::byte>;
		using binary_output_file_stream = std::basic_ofstream<std::byte>;

		using openmode = std::ios::openmode;

		/*****************************************************************************************
		*
		*                                       concepts
		*
		*****************************************************************************************/
		template<typename T> concept default_constructible = std::is_default_constructible_v<T>;
		template<typename T> concept fp = std::floating_point<T>;
		template<typename T> concept arithmetic = fp<T> || std::integral<T>;
		template<typename It> concept io_iterator = std::input_or_output_iterator<It>;
		template<typename It> concept not_io_iterator = !io_iterator<It>;
		template<typename T> concept not_range = !std::ranges::range<T>;
		template<typename It> concept real_value_iterator = io_iterator<It> && fp<iterator_value_type<It>>;
		template<typename Rng> concept real_value_range = std::ranges::range<Rng> && fp<range_value_type<Rng>>;
		template<typename T, typename... Ts> concept constrained_on = (std::is_same_v<T, Ts> || ...);
		template<typename F, typename... Args> concept procedure = std::invocable<F, Args...>&& std::is_void_v<std::invoke_result_t<F, Args...>>;
		template<typename F, typename... Args> concept real_value_function = std::invocable<F, Args...>&& fp<std::invoke_result_t<F, Args...>>;

		/*****************************************************************************************
		*
		*                                  numerical constants
		*
		*****************************************************************************************/
		template<typename T> constexpr T minimum() { return std::numeric_limits<T>::min(); }
		template<typename T> constexpr T maximum() { return std::numeric_limits<T>::max(); }
		template<fp T> constexpr T epsilon() { return std::numeric_limits<T>::epsilon(); }
		constexpr decltype(auto) pi32() { return std::numbers::pi_v<real::single_precision::type>; }
		constexpr decltype(auto) pi64() { return std::numbers::pi_v<real::double_precision::type>; }
		constexpr decltype(auto) e32() { return std::numbers::e_v<real::single_precision::type>; }
		constexpr decltype(auto) e64() { return std::numbers::e_v<real::double_precision::type>; }

		/*****************************************************************************************
		*
		*                                      conversions
		*
		*****************************************************************************************/
		namespace convert
		{
			template<fp T> constexpr T degrees_to_radians(T x) { return x * std::numbers::pi_v<T> / (T)180; }
			template<fp T> constexpr T radians_to_degrees(T x) { return x * (T)180 / std::numbers::pi_v<T>; }
			template<fp T> constexpr T Celsius_to_Fahrenheit(T x) { return (x * (T)9 / (T)5) + (T)32; }
			template<fp T> constexpr T Fahrenheit_to_Celsius(T x) { return (x - (T)32) * (T)5 / (T)9; }
			template<std::integral I> std::vector<unsigned_integer::_8bit::type> decimal_to_binary(I n)
			{
				std::vector<unsigned_integer::_8bit::type> binval{};
				binval.reserve(64);
				if (n == (I)0)
					binval.emplace_back('0');
				else
				{
					while (n != (I)0)
					{
						binval.emplace_back(unsigned_integer::_8bit::value(n % (I)2));
						n /= (I)2;
					}
					binval.emplace_back('b');
					binval.emplace_back('0');
					std::ranges::reverse(std::ranges::begin(binval), std::ranges::end(binval));
				}
				return binval;
			}
			template<std::integral I> std::vector<char> decimal_to_hexadecimal(I n)
			{
				std::vector<char> hexval{};
				hexval.reserve(64);
				if (n == (I)0)
				{
					hexval.emplace_back('0');
					hexval.emplace_back('x');
					hexval.emplace_back('0');
				}
				else
				{
					while (n != (I)0)
					{
						auto rem{ n % (I)16 };
						switch (rem)
						{
						case (I)10:
							hexval.emplace_back('A');
							break;
						case (I)11:
							hexval.emplace_back('B');
							break;
						case (I)12:
							hexval.emplace_back('C');
							break;
						case (I)13:
							hexval.emplace_back('D');
							break;
						case (I)14:
							hexval.emplace_back('E');
							break;
						case (I)15:
							hexval.emplace_back('F');
							break;
						default:
							break;
						}
						n /= (I)16;
					}
					hexval.emplace_back('x');
					hexval.emplace_back('0');
					std::ranges::reverse(std::ranges::begin(hexval), std::ranges::end(hexval));
				}
				return hexval;
			}
		}
		template<fp T> constexpr decltype(auto) deg2rad(T x) { return convert::degrees_to_radians(x); }
		template<fp T> constexpr decltype(auto) rad2deg(T x) { return convert::radians_to_degrees(x); }
		template<fp T> constexpr decltype(auto) C2F(T x) { return convert::Celsius_to_Fahrenheit(x); }
		template<fp T> constexpr decltype(auto) F2C(T x) { return convert::Fahrenheit_to_Celsius(x); }

		// RAII (Resource Acquisition Is Initialization) namespace
		namespace raii
		{
			// input file
			template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
			struct basic_input_file
			{
				std::basic_ifstream<Enc, EncTraits> stream;
				basic_input_file() :stream{} {}
				virtual ~basic_input_file()
				{
					if (stream.is_open())
						stream.close();
				}
				inline void open(std::filesystem::path fp, openmode opm = std::ios::in)
				{
					if (stream.is_open()) stream.close();
					stream.open(fp, std::ios::in | opm);
					if (!stream.is_open())
					{
						native::narrow_encoded::string errmsg{ "ERROR: Cannot open " };
						errmsg += fp.string();
						errmsg += native::narrow_encoded::string{ " file." };
						throw std::runtime_error(errmsg.c_str());
					}
				}
				constexpr std::basic_string<Enc, EncTraits, EncAlloc> read(std::filesystem::path filepath, openmode opm = std::ios::in)
				{
					using input_stream_buffer_iterator = std::istreambuf_iterator<Enc, EncTraits>;
					open(filepath, opm);
					std::basic_string<Enc, EncTraits, EncAlloc> file_contents{};
					stream.seekg(0, std::ios::end);
					file_contents.reserve(stream.tellg());
					stream.seekg(0, std::ios::beg);
					file_contents.assign(input_stream_buffer_iterator(stream), input_stream_buffer_iterator());
					return file_contents;
				}
				constexpr std::vector<std::basic_string<Enc, EncTraits, EncAlloc>> read_line_by_line(std::filesystem::path filepath, openmode opm = std::ios::in)
				{
					std::vector<std::basic_string<Enc, EncTraits, EncAlloc>> file_lines{};
					open(filepath, opm);
					std::basic_string<Enc, EncTraits, EncAlloc> line{};
					stream.seekg(0, std::ios::end);
					file_lines.reserve(stream.tellg());
					stream.seekg(0, std::ios::beg);
					while (!stream.eof())
					{
						std::getline<Enc, EncTraits, EncAlloc>(stream, line);
						file_lines.emplace_back(line);
					}
					return file_lines;
				}
			};
			template<> struct basic_input_file<std::byte>
			{
				binary_input_file_stream stream;
				basic_input_file() :stream{} {}
				virtual ~basic_input_file()
				{
					if (stream.is_open())
						stream.close();
				}
				inline void open(std::filesystem::path fp, openmode opm = std::ios::in)
				{
					if (stream.is_open()) stream.close();
					stream.open(fp, std::ios::in | std::ios::binary | opm);
					if (!stream.is_open())
					{
						native::narrow_encoded::string errmsg{ "ERROR: Cannot open " };
						errmsg += fp.string();
						errmsg += native::narrow_encoded::string{ " file." };
						throw std::runtime_error(errmsg.c_str());
					}
				}
				inline binary_string read(std::filesystem::path filepath, openmode opm = std::ios::in)
				{
					using input_stream_buffer_iterator = std::istreambuf_iterator<std::byte>;
					open(filepath, opm);
					binary_string file_contents{};
					stream.seekg(0, std::ios::end);
					file_contents.reserve(stream.tellg());
					stream.seekg(0, std::ios::beg);
					file_contents.assign(input_stream_buffer_iterator(stream), input_stream_buffer_iterator());
					return file_contents;
				}
				inline std::vector<binary_string> read_line_by_line(std::filesystem::path filepath, openmode opm = std::ios::in)
				{
					std::vector<binary_string> file_lines{};
					open(filepath, opm);
					binary_string line{};
					stream.seekg(0, std::ios::end);
					file_lines.reserve(stream.tellg());
					stream.seekg(0, std::ios::beg);
					while (!stream.eof())
					{
						std::getline<std::byte>(stream, line);
						file_lines.emplace_back(line);
					}
					return file_lines;
				}
			};

			// output file
			template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
			struct basic_output_file
			{
				std::basic_ofstream<Enc, EncTraits> stream;
				basic_output_file() :stream{} {}
				virtual ~basic_output_file()
				{
					if (stream.is_open())
					{
						stream.flush();
						stream.close();
					}
				}
				inline void open(std::filesystem::path fp, openmode opm = std::ios::out)
				{
					if (stream.is_open())
					{
						stream.flush();
						stream.close();
					}
					stream.open(fp, std::ios::out | opm);
					if (!stream.is_open())
					{
						native::narrow_encoded::string errmsg{ "ERROR: Cannot open " };
						errmsg += fp.string();
						errmsg += native::narrow_encoded::string{ " file." };
						throw std::runtime_error(errmsg.c_str());
					}
				}
			};
			template<> struct basic_output_file<std::byte>
			{
				std::basic_ofstream<std::byte> stream;
				basic_output_file() :stream{} {}
				virtual ~basic_output_file()
				{
					if (stream.is_open())
					{
						stream.flush();
						stream.close();
					}
				}
				inline void open(std::filesystem::path fp, openmode opm = std::ios::out | std::ios::binary)
				{
					if (stream.is_open())
					{
						stream.flush();
						stream.close();
					}
					stream.open(fp, std::ios::out | std::ios::binary | opm);
					if (!stream.is_open())
					{
						native::narrow_encoded::string errmsg{ "ERROR: Cannot open " };
						errmsg += fp.string();
						errmsg += native::narrow_encoded::string{ " file." };
						throw std::runtime_error(errmsg.c_str());
					}
				}
			};
		}

		// std::vector<bool> output stream operator overloadings
		template<typename Enc, typename EncTraits = std::char_traits<Enc>>
		constexpr raii::basic_output_file<Enc, EncTraits>& operator<<(raii::basic_output_file<Enc, EncTraits>& output_file, const std::vector<bool>& v)
		{
			for (auto i : std::ranges::views::iota(Size::value(0), v.size()))
				switch (v[i])
				{
				case true:
					output_file.stream << '1';
					break;
				default:
					output_file.stream << '0';
					break;
				}
			output_file.stream << '\n';
			return output_file;
		}
		template<typename Enc, typename EncTraits = std::char_traits<Enc>>
		constexpr raii::basic_output_file<Enc, EncTraits>& operator<<(raii::basic_output_file<Enc, EncTraits>& output_file, const dynamic_array<bool>& v)
		{
			for (auto i : std::ranges::views::iota(Size::value(0), v.size()))
				switch (v[i])
				{
				case true:
					output_file.stream << '1';
					break;
				default:
					output_file.stream << '0';
					break;
				}
			output_file.stream << '\n';
			return output_file;
		}

		namespace native
		{
			namespace narrow_encoded
			{
				using input_file = raii::basic_input_file<native::narrow_encoding>;
				using output_file = raii::basic_output_file<native::narrow_encoding>;
			}
			namespace wide_encoded
			{
				using input_file = raii::basic_input_file<native::wide_encoding>;
				using output_file = raii::basic_output_file<native::wide_encoding>;
			}
		}
		namespace utf8
		{
			using input_file = raii::basic_input_file<utf8_encoding>;
			using output_file = raii::basic_output_file<utf8_encoding>;
		}		
		using binary_input_file = raii::basic_input_file<std::byte>;
		using binary_output_file = raii::basic_output_file<std::byte>;

		// filesystem path data structure
		struct filesystem_path
		{
			// filesystem path serializers
			inline void serialize(std::filesystem::path filepath, binary_string& buffer)
			{
				buffer.clear();
				buffer.reserve(Size::value(1'073'741'824));
				auto serialize_directory = [&buffer](std::filesystem::path p)
					{
						// phase 1: watermark as a directory
						buffer.push_back(static_cast<std::byte>(true));

						// phase 2: serialize directory name size 
						auto directory_name{ p.wstring() };
						auto directory_name_size{ directory_name.size() };
						binary_string directory_name_size_subbuffer(sizeof(Size::type), std::byte{});
						memcpy(directory_name_size_subbuffer.data(), &directory_name_size, sizeof(Size::type));
						buffer.insert_range(buffer.cend(), std::move(directory_name_size_subbuffer));

						// phase 3: serialize directory name itself
						binary_string directory_name_subbuffer(directory_name_size * sizeof(wchar_t), std::byte{});
						memcpy(directory_name_subbuffer.data(), directory_name.data(), directory_name_size * sizeof(wchar_t));
						buffer.insert_range(buffer.cend(), std::move(directory_name_subbuffer));
					};
				auto serialize_regular_file = [&buffer](std::filesystem::path p)
					{
						// phase 1: watermark as a file
						buffer.push_back(static_cast<std::byte>(false));

						// phase 2: serialize file name size
						auto file_name{ p.wstring() };
						auto file_name_size{ file_name.size() };
						binary_string file_name_size_subbuffer(sizeof(Size::type), std::byte{});
						memcpy(file_name_size_subbuffer.data(), &file_name_size, sizeof(Size::type));
						buffer.insert_range(buffer.cend(), std::move(file_name_size_subbuffer));

						// phase 3: serialize file name itself
						binary_string file_name_subbuffer(file_name_size * sizeof(wchar_t), std::byte{});
						memcpy(file_name_subbuffer.data(), file_name.data(), file_name_size * sizeof(wchar_t));
						buffer.insert_range(buffer.cend(), std::move(file_name_subbuffer));

						// phase 4: serialize file contents size
						auto file_contents_size{ std::filesystem::file_size(p) };
						binary_string file_contents_size_subbuffer(sizeof(Size::type), std::byte{});
						memcpy(file_contents_size_subbuffer.data(), &file_contents_size, sizeof(Size::type));
						buffer.insert_range(buffer.cend(), std::move(file_contents_size_subbuffer));

						// phase 5: serialize file contents themselves
						binary_string file_contents_subbuffer{};
						binary_input_file().read(p).swap(file_contents_subbuffer);
						buffer.insert_range(buffer.cend(), std::move(file_contents_subbuffer));
					};
				std::stack<std::filesystem::path> s{};
				s.push(filepath);
				while (!s.empty())
				{
					auto p = s.top();
					s.pop();

					// phase 0: check the filesystem path whether it is a directory or a file
					if (std::filesystem::is_directory(p))
					{
						serialize_directory(p);
						for (auto& entry : std::filesystem::directory_iterator(p))
							s.push(entry.path());
					}
					else if (std::filesystem::is_regular_file(p))
						serialize_regular_file(p);
				}
			}
			inline void serialize(std::filesystem::path filepath)
			{
				binary_string buffer{};
				serialize(filepath, buffer);
				binary_output_file output_file{};
				output_file.open(filepath.wstring() + native::wide_encoded::string(L".serialized"));
				for (auto& e : buffer)
					output_file.stream << e;
			}

			// filesystem path deserializer
			inline void deserialize(std::filesystem::path serialized_filepath)
			{
				binary_string buffer{};

				binary_input_file().read(serialized_filepath).swap(buffer);

				auto it{ buffer.begin() };

				// phase 0: check the boolean flag whether there is a directory or a file is in the following
				while (it != buffer.end())
				{
					auto is_directory = static_cast<bool>(*it);
					it = std::ranges::next(it);

					// if there is a directory in the following
					if (is_directory)
					{
						// phase 1: directory name size extraction from binary serialized file
						std::span<std::byte> directory_name_size_span(it, sizeof(Size::type));
						Size::type directory_name_size{};
						memcpy(&directory_name_size, directory_name_size_span.data(), directory_name_size_span.size());
						it = std::ranges::next(it, directory_name_size_span.size());

						// phase 2: directory name extraction from binary serialized file (in the size of directory_name_size)
						std::span<std::byte> directory_name_span(it, directory_name_size * sizeof(wchar_t));
						native::wide_encoded::string directory_name(directory_name_size, wchar_t{});
						memcpy(directory_name.data(), directory_name_span.data(), directory_name_span.size());
						it = std::ranges::next(it, directory_name_span.size());

						// phase 3: create the directory with the above extracted name
						std::filesystem::create_directory(std::filesystem::path(directory_name));
					}
					else// if there is a file in the following
					{
						// phase 1: file name size extraction from binary serialized file
						std::span<std::byte> file_name_size_span(it, sizeof(Size::type));
						Size::type file_name_size{};
						memcpy(&file_name_size, file_name_size_span.data(), file_name_size_span.size());
						it = std::ranges::next(it, file_name_size_span.size());

						// phase 2: file name extraction from binary serialized file (in the size of file_name_size)
						std::span<std::byte> file_name_span(it, file_name_size * sizeof(wchar_t));
						native::wide_encoded::string file_name(file_name_size, wchar_t{});
						memcpy(file_name.data(), file_name_span.data(), file_name_span.size());
						it = std::ranges::next(it, file_name_span.size());

						// phase 3: file contents size extraction from binary serialized file
						std::span<std::byte> file_contents_size_span(it, sizeof(Size::type));
						Size::type file_contents_size{};
						memcpy(&file_contents_size, file_contents_size_span.data(), file_contents_size_span.size());
						it = std::ranges::next(it, file_contents_size_span.size());

						// phase 4: file contents extraction from binary serialized file (in the size of file_contents_size)
						std::span<std::byte> file_contents_span(it, file_contents_size);
						binary_output_file output_file{};
						output_file.open(file_name);
						std::ranges::for_each(file_contents_span, [&output_file](auto& elem) { output_file.stream << elem; });
						it = std::ranges::next(it, file_contents_span.size());
					}
				}
			}
			
			// filesystem path copy in ascending counted numbers if the path already existed
			struct copy_in_ascending_counted_numbers_if_the_path_already_existed
			{
				inline void copy_regular_file(std::filesystem::path source_path, std::filesystem::path destination_path)
				{
					std::filesystem::copy(source_path, [&destination_path]()
						{
							std::filesystem::path new_destination_filepath{ destination_path };
							const auto destination_parent_path{ destination_path.parent_path() };
							const auto destination_stem{ destination_path.stem().wstring() };
							const auto destination_extension{ destination_path.extension().wstring() };
							auto counter{ Size::value(0) };
							while (std::filesystem::exists(new_destination_filepath))
							{
								new_destination_filepath.clear();
								new_destination_filepath = destination_parent_path /
									std::filesystem::path(destination_stem +
										native::wide_encoded::string{ L" (" } +
										std::to_wstring(++counter) +
										native::wide_encoded::string{ L")" } +
										destination_extension);
							}
							return new_destination_filepath;
						}());
				}
				inline void copy_directory(std::filesystem::path source_path, std::filesystem::path destination_path)
				{
					auto new_destination_path{ destination_path };
					auto counter{ Size::value(0) };
					while (std::filesystem::exists(new_destination_path))
					{
						new_destination_path.clear();
						new_destination_path = destination_path / std::filesystem::path(source_path.filename().wstring() +
							native::wide_encoded::string{ L" (" } +
							std::to_wstring(++counter) +
							native::wide_encoded::string{ L")" });
					}
					std::filesystem::create_directories(new_destination_path);
					std::stack<std::pair<std::filesystem::path, std::filesystem::path>> stk{};
					for (auto& entry : std::filesystem::directory_iterator(source_path))
					{
						auto source_child_path{ entry.path() };
						auto new_destination_child_path = new_destination_path / source_child_path.filename();
						stk.push(std::make_pair(source_child_path, new_destination_child_path));
					}
					while (!stk.empty())
					{
						auto [src, dst] = stk.top();
						stk.pop();
						if (std::filesystem::is_directory(src))
						{
							std::filesystem::create_directories(dst);
							for (auto& entry : std::filesystem::directory_iterator(src))
							{
								auto src_child_path{ entry.path() };
								auto dst_child_path = dst / src_child_path.filename();
								stk.push(std::make_pair(src_child_path, dst_child_path));
							}
						}
						else if (std::filesystem::is_regular_file(src))
							copy_regular_file(src, dst);
					}
				}				
				inline void operator()(std::filesystem::path src, std::filesystem::path dst)
				{
					if (std::filesystem::is_directory(src))
						copy_directory(src, dst);
					else if (std::filesystem::is_regular_file(src))
						copy_regular_file(src, dst);
				}
			};
		};
		
		// asynchronous (coroutine) traits
		namespace asynchronous
		{
			// resumable generator data structure
			template<typename T> struct generator
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
					constexpr pointer operator->() { return addressof(operator*()); }
					constexpr const_reference operator*() const { return handle.promise().current_value; }
					constexpr pointer operator->() const { return addressof(operator*()); }
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

			// awaitable task data structure
			template<typename T> struct task
			{
				struct promise_type
				{
					std::variant<std::monostate, T, std::exception_ptr> result;
					continuation_type continuation;
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
				constexpr decltype(auto) await_suspend(continuation_type c)
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
					continuation_type continuation;
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
				inline decltype(auto) await_suspend(continuation_type c)
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
			template<typename T>
			using result_type = decltype(std::declval<T&>().await_resume());

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
			template<typename T> struct sync_wait_task
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
			template<typename T> result_type<T> sync_wait(T&& task)
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

		// combinatorial tree node type
		struct combinatorial_tree_node
		{
			signed_integer::_8bit::type index;
			bool state;
			combinatorial_tree_node* left;
			combinatorial_tree_node* right;
			combinatorial_tree_node()
				:index{ signed_integer::_8bit::value(-1) },
				state{ false },
				left{ nullptr },
				right{ nullptr }
			{
			}
		};

		// implementing all of the combinations of a range elements
		template<io_iterator It> constexpr decltype(auto) get_combinations(It begin, It end)
		{
			auto combinatorial_tree_node_generator = []() -> asynchronous::generator<combinatorial_tree_node*>
				{
					std::vector<combinatorial_tree_node> combinatorial_tree_nodes{};
					combinatorial_tree_nodes.reserve(Size::value::cast(1'024));
					while (true)
					{
						combinatorial_tree_nodes.emplace_back(combinatorial_tree_node{});
						co_yield &combinatorial_tree_nodes.back();
					}
				};

			auto combinatorial_tree_node_store = combinatorial_tree_node_generator();

			// type alias
			using T = iterator_value_type<It>;
			using ptr = T*;

			// variables
			auto number_of_indexes{ signed_integer::_8bit::value(std::ranges::distance(begin, end)) };

			// build combinarorial tree
			combinatorial_tree_node_store.resume();
			auto root = combinatorial_tree_node_store.get_value();
			std::stack<combinatorial_tree_node*> q{};
			q.push(root);
			while (!q.empty())
			{
				auto r = q.top();
				if (r->index < number_of_indexes - signed_integer::_8bit::value(1))
				{
					q.pop();
					combinatorial_tree_node_store.resume();
					r->right = combinatorial_tree_node_store.get_value();
					r->right->index = r->index + signed_integer::_8bit::value(1);
					r->right->state = true;
					q.push(r->right);
					combinatorial_tree_node_store.resume();
					r->left = combinatorial_tree_node_store.get_value();
					r->left->index = r->index + signed_integer::_8bit::value(1);
					r->left->state = false;
					q.push(r->left);
				}
				else
					q.pop();
			}

			// generate all combinations of a number_of_indexes-element set in bit pattern
			table<bool> bit_patterns{};
			bit_patterns.reserve([&]()
				{
					auto s = Size::value(1);
					for (auto i : std::ranges::views::iota(signed_integer::_8bit::value(0), number_of_indexes))
						s *= Size::value(2);
					return s;
				}());
			std::vector<bool> buffer(Size::value(number_of_indexes), false);
			q.push(root->right);
			q.push(root->left);
			while (!q.empty())
			{
				auto r = q.top();
				q.pop();
				if (r->right && r->left)
				{
					buffer[r->index] = r->state;
					q.push(r->right);
					q.push(r->left);
				}
				else
				{
					buffer[r->index] = r->state;
					bit_patterns.emplace_back(buffer);
				}
			}

			// all of the combinations
			table<ptr> all_combinations(bit_patterns.size(), std::vector<ptr>{});
			std::transform(
				std::execution::par,
				bit_patterns.cbegin(),
				bit_patterns.cend(),
				all_combinations.begin(),
				[begin](const std::vector<bool>& bit_pattern_i)
				{
					auto m{ bit_pattern_i.size() };
					std::vector<ptr> combination_i{};
					combination_i.reserve(m);
					for (auto j : std::ranges::views::iota(Size::value(0), m))
					{
						if (bit_pattern_i[j])
							combination_i.emplace_back(&(*std::ranges::next(begin, j)));
					}
					return combination_i;
				}
			);
			return std::make_pair(std::move(all_combinations), std::move(bit_patterns));
		}

		// refill ranges
		template<io_iterator It> constexpr void refill(It beg, It end, iterator_value_type<It> newval)
		{
			std::ranges::for_each(beg, end, [&](auto& elem) { elem = newval; });
		}

		// add values in a range
		template<io_iterator It> constexpr iterator_value_type<It> add(It b, It e)
		{
			using value_type = iterator_value_type<It>;
			return std::reduce(std::execution::seq, b, e, (value_type)0, std::plus<value_type>());
		}

		// multiplies values in a range
		template<io_iterator It> constexpr iterator_value_type<It> multiply(It b, It e)
		{
			using value_type = iterator_value_type<It>;
			return std::reduce(std::execution::seq, b, e, (value_type)1, std::multiplies<value_type>());
		}

		// Arithmetic mean calculator
		template<real_value_iterator Iter> constexpr iterator_value_type<Iter> arithmetic_mean(Iter b, Iter e)
		{
			using value_type = iterator_value_type<Iter>;
			auto mu{ static_cast<value_type>(0) };
			auto size{ std::ranges::distance(b, e) };
			if (size == Size::value(1))
				mu = *b;
			else
				mu = add(b, e) / std::ranges::distance(b, e);
			return mu;
		}

		// Arithmetic mean + standard deviation calculator
		template<real_value_iterator Iter>
		constexpr std::pair<iterator_value_type<Iter>, iterator_value_type<Iter>> arithmetic_mean_and_standard_deviation(Iter b, Iter e)
		{
			using value_type = iterator_value_type<Iter>;
			auto mu{ arithmetic_mean(b, e) };
			auto sigma{ static_cast<value_type>(0) };
			auto size{ std::ranges::distance(b, e) };
			if (size > Size::value(1))
				sigma = std::sqrt(std::transform_reduce(std::execution::seq,
					b,
					e,
					(value_type)0,
					std::plus<value_type>(),
					[mu](const auto& v) { return abs(v - mu) * abs(v - mu); })) / (size - Size::value(1));
			return std::make_pair(mu, sigma);
		}

		// normalizer
		template<real_value_iterator Iter> constexpr void normalize(Iter b, Iter e)
		{
			using value_type = iterator_value_type<Iter>;
			auto N{ std::sqrt(std::transform_reduce(
					std::execution::par_unseq,
					b,
					e,
					(value_type)0,
					std::plus<value_type>(),
					[](value_type& e) { return e * e; }
				))
			};
			std::for_each(
				std::execution::par_unseq,
				b,
				e,
				[N](value_type& e) { e /= N; }
			);
		}

		// probability distribution simulators
		template<real_value_iterator It> constexpr void simulate_beta_distribution_function(
			iterator_value_type<It> alpha,
			iterator_value_type<It> beta,
			std::mt19937_64& rng,
			It beg,
			It end
		)
		{
			// beta distributor
			auto distributor = [&]()
				{
					// beta distribution parameters assertion
					// parameters check (alpha > 0.0, beta > 0.0)
					if (alpha <= iterator_value_type<It>{ 0 })
						throw std::runtime_error("alpha parameter (shape parameter) in beta distribution function must be greater than zero.");
					if (beta <= iterator_value_type<It>{ 0 })
						throw std::runtime_error("beta parameter (shape parameter) in beta distribution function must be greater than zero.");
					std::gamma_distribution<real::double_precision::type> parametrized_gamma_distribution_alpha(alpha, iterator_value_type<It>{ 1 });
					std::gamma_distribution<real::double_precision::type> parametrized_gamma_distribution_beta(beta, iterator_value_type<It>{ 1 });
					return [parametrized_gamma_distribution_alpha, parametrized_gamma_distribution_beta](auto& random_number_generator) mutable
						{
							auto _{ parametrized_gamma_distribution_alpha(random_number_generator) };
							return _ / (_ + parametrized_gamma_distribution_beta(random_number_generator));
						};
				};
			auto rnd{ distributor() };
			std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
		}
		template<real_value_iterator It> constexpr void simulate_gamma_distribution_function(
			iterator_value_type<It> alpha,
			iterator_value_type<It> beta,
			std::mt19937_64& rng,
			It beg,
			It end
		)
		{
			// gamma distributor
			auto distributor = [&]()
				{
					// gamma distribution parameters assertion
					// parameters check (alpha > 0.0, beta > 0.0)
					if (alpha <= iterator_value_type<It>{ 0 })
						throw std::runtime_error("alpha Parameter (shape Parameter) in gamma distribution function must be greater than zero.");
					if (beta <= iterator_value_type<It>{ 0 })
						throw std::runtime_error("beta Parameter (scale Parameter) in gamma distribution function must be greater than zero.");
					std::gamma_distribution<real::double_precision::type> parametrized_gamma_distribution(alpha, iterator_value_type<It>{ 1 } / beta);
					return [parametrized_gamma_distribution](auto& random_number_generator) mutable
						{
							return parametrized_gamma_distribution(random_number_generator);
						};
				};
			auto rnd{ distributor() };
			std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
		}
		template<real_value_iterator It> constexpr void simulate_normal_distribution_function(
			iterator_value_type<It> mu,
			iterator_value_type<It> sigma,
			std::mt19937_64& rng,
			It beg,
			It end
		)
		{
			// normal distributor
			auto distributor = [&]()
				{
					// normal distribution parameters assertion
					// Parameter checking (0.0 <= mu <= 1.0)
					if (mu < iterator_value_type<It>{ 0 } || mu > iterator_value_type<It>{ 1 })
						throw std::runtime_error("mean value Parameter (mu) in normal distribution function"
							" must be between zero and one (it's a probability).");
					std::normal_distribution<real::double_precision::type> parametrized_normal_distribution(mu, sigma);
					return [parametrized_normal_distribution](auto& random_number_generator) mutable
						{
							return parametrized_normal_distribution(random_number_generator);
						};
				};
			auto rnd{ distributor() };
			std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
		}
		template<real_value_iterator It> constexpr void simulate_lognormal_distribution_function(
			iterator_value_type<It> EV,
			iterator_value_type<It> EF,
			std::mt19937_64& rng,
			It beg,
			It end
		)
		{
			// log-normal distributor
			auto distributor = [&]()
				{
					// log-normal distribution parameters assertion
					// parameter checking (2nd Parameter - error factor - must be greater than 0.0)
					if (EF <= iterator_value_type<It>{ 0 })
						throw std::runtime_error("error factor parameter in log-normal distribution function"
							" must be greater than zero.");
					auto sigma = (std::log(EF) / iterator_value_type<It>{ 1.6449 });
					auto mu = (std::log(EV) - (sigma * sigma / iterator_value_type<It>{ 2 }));
					std::lognormal_distribution<real::double_precision::type> parametrized_lognormal_distribution(mu, sigma);
					return [parametrized_lognormal_distribution](auto& random_number_generator) mutable
						{
							return parametrized_lognormal_distribution(random_number_generator);
						};
				};
			auto rnd{ distributor() };
			std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
		}
		template<real_value_iterator It> constexpr void simulate_uniform_distribution_function(
			iterator_value_type<It> minval,
			iterator_value_type<It> maxval,
			std::mt19937_64& rng,
			It beg,
			It end
		)
		{
			// uniform distributor
			auto distributor = [&]()
				{
					// uniform distribution parameters assertion
					// Parameter checking (both parameters must be between zero and one)
					if (minval < iterator_value_type<It>{ 0 } || minval > iterator_value_type<It>{ 1 })
						throw std::runtime_error("minimum value Parameter in uniform distribution function"
							" must be between zero and one (it's a probability).");
					if (maxval < iterator_value_type<It>{ 0 } || maxval > iterator_value_type<It>{ 1 })
						throw std::runtime_error("maximum value Parameter uniform distribution function"
							" must be between zero and one (it's a probability).");
					std::uniform_real_distribution<real::double_precision::type> parametrized_uniform_distribution(minval, maxval);
					return [parametrized_uniform_distribution](auto& random_number_generator) mutable
						{
							return parametrized_uniform_distribution(random_number_generator);
						};
				};
			auto rnd{ distributor() };
			std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
		}
		template<real_value_iterator It> constexpr void simulate_loguniform_distribution_function(
			iterator_value_type<It> minval,
			iterator_value_type<It> maxval,
			std::mt19937_64& rng,
			It beg,
			It end
		)
		{
			// log-uniform distributor
			auto distributor = [&]()
				{
					// log-uniform distribution parameters assertion
					// parameter checking (both parameters must be greater than zero, 
						// because, there is no value for negative logarithms)
					if (minval <= iterator_value_type<It>{ 0 })
						throw std::runtime_error("minimum value parameter in log-uniform distribution function"
							" must be greater than zero (there is no value for zero-negative logarithms)");
					if (maxval <= iterator_value_type<It>{ 0 })
						throw std::runtime_error("maximum value parameter in log-uniform distribution function"
							" must be greater than zero (there is no value for zero-negative logarithms)");
					std::uniform_real_distribution<real::double_precision::type> parametrized_loguniform_distribution(minval, maxval);
					return [parametrized_loguniform_distribution](auto& random_number_generator) mutable
						{
							return std::exp(parametrized_loguniform_distribution(random_number_generator));
						};
				};
			auto rnd{ distributor() };
			std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
		}

		// integral types existence template class
		template<typename I> class is_there
		{
		public:
			is_there(I value) :m_value{ value } {}
			virtual ~is_there() = default;
			template<I... Vals> constexpr bool in() { return ((m_value == Vals) || ...); }
		private:
			I m_value;
		};

		// factorial calculator
		template<std::integral I> constexpr I factorial(I p)
		{
			using T = decltype(p);
			constexpr auto zero{ static_cast<T>(0) };
			constexpr auto one{ static_cast<T>(1) };
			auto result{ zero };
			if (p == zero)
				result = one;
			else
			{
				auto r{ p };
				auto l{ p - one };
				while (l >= one)
				{
					r *= l;
					l--;
				}
				result = r;
			}
			return result;
		}

		// number of total combinations calculator
		template<std::integral I> constexpr I nCk(I n, I k) { return ((factorial(n) / (factorial(n - k) * factorial(k)))); }

		/*****************************************************************************************
		*
		*                                   CMakeLists.txt
		*
		*****************************************************************************************/
		const native::narrow_encoded::string CMakeLists{
R"(
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
file(GLOB src "src/t00lb0x.cpp src/t00lb0x.h")
project(t00lb0x)
# common compilation flags
set(CMAKE_CXX_FLAGS "-Wall -Werror")
# debug compilation flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")
# release compilation flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -pedantic")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(build/debug/t00lb0x ${src})
else()
	add_executable(build/release/t00lb0x ${src})
endif()
)"
		};

		/*****************************************************************************************
		*
		*                                    iterator traits
		*
		*****************************************************************************************/
		// iterate through a range step sized
		template<io_iterator It, std::invocable<iterator_value_type<It>&> F>
		constexpr auto iterate(It begin, Size::type n, Size::type step_size, F&& f) -> void
		{
			auto c = Size::value::_0;
			auto it = begin;
			do
			{
				std::invoke(std::forward<F>(f), *it);
				c++;
			} while (c < n && [&]() { it = std::ranges::next(it, step_size); return true; }());
		}

		// frequencies of a range elements
		template<std::ranges::range Rng> constexpr decltype(auto) frequencies(const Rng& rng)
		{
			hash_map<
				std::reference_wrapper<range_value_type<Rng>>,
				Size::type,
				std::hash<std::reference_wrapper<range_value_type<Rng>>>,
				std::equal_to<std::reference_wrapper<range_value_type<Rng>>>,
				std::allocator<std::pair<const std::reference_wrapper<range_value_type<Rng>>, Size::type>>
			> h{};
			for (const auto& elem : rng)
				h[std::cref(elem)]++;
			return h;
		}
		template<io_iterator It> constexpr decltype(auto) frequencies(It beg, It end)
		{
			hash_map <
				std::reference_wrapper<iterator_value_type<It>>,
				Size::type,
				std::hash<std::reference_wrapper<iterator_value_type<It>>>,
				std::equal_to<std::reference_wrapper<iterator_value_type<It>>>,
				std::allocator<std::pair<const std::reference_wrapper<iterator_value_type<It>>, Size::type>>
			> h{};
			std::ranges::for_each(beg, end, [&](const auto& elem) { h[std::cref(elem)]++; });
			return h;
		}

		// shuffle a range and sample it n times
		template<io_iterator It> constexpr std::vector<iterator_value_type<It>> do_n_times_shuffle_and_sample(It beg, It end, Size::type n)
		{
			std::random_device rd{};
			std::mt19937_64 rng{ rd() };
			std::vector<iterator_value_type<It>> samples(n);
			auto it{ std::ranges::begin(samples) };
			while (it != std::ranges::end(samples))
			{
				std::ranges::shuffle(beg, end, rng);
				std::ranges::sample(beg, end, it, 1, rng);
				it++;
			}
			return samples;
		}

		/*****************************************************************************************
		*
		*                                     memory traits
		*
		*****************************************************************************************/
		// decimal address reader
		template<not_io_iterator T> constexpr decltype(auto) decimal_memory_address(T& obj) { return reinterpret_cast<uintptr_t>(&obj); }
		template<not_io_iterator T> constexpr decltype(auto) decimal_memory_address(T* obj) { return reinterpret_cast<uintptr_t>(&(*obj)); }
		template<io_iterator It> constexpr decltype(auto) decimal_memory_address(It it) { return reinterpret_cast<uintptr_t>(&(*it)); }
		template<io_iterator It> constexpr decltype(auto) decimal_memory_address(native::narrow_encoded::output_file& of, It beg, It end)
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
		template<io_iterator It> constexpr decltype(auto) decimal_memory_address(native::wide_encoded::output_file& of, It beg, It end)
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
		template<default_constructible T, Size::type ChunkSize> class object_pool
		{
		public:
			using value_type = T;
			using type = object_pool<value_type, ChunkSize>;
			using data_structure = table<value_type>;
		private:
			alignas(value_type) data_structure object_pool_;
			Size::type counter_;
			constexpr void allocate_chunk() { object_pool_.emplace_back(std::vector<value_type>(ChunkSize, value_type())); }
		public:
			object_pool()
				:object_pool_{},
				counter_{ Size::value(0) }
			{
				object_pool_.reserve(Size::value(2) * ChunkSize);
				allocate_chunk();
			}
			object_pool(const type&) = delete;
			object_pool(type&&) noexcept = delete;
			virtual ~object_pool() = default;
			auto operator=(const type&)->type & = delete;
			auto operator=(type&&) noexcept -> type & = delete;
			bool operator==(const object_pool& other) const = delete;
			auto get_object() -> value_type*
			{
				if (counter_ == ChunkSize)
				{
					allocate_chunk();
					counter_ = Size::value(0);
				}
				return &object_pool_.back()[counter_++];
			}
		};

		// single-thread fixed-size memory manager (arena)
		template<Size::type N> class arena
		{
		private:
			static constexpr inline const Size::type alignment = alignof(std::max_align_t);
			static inline Size::type align_up(Size::type n) noexcept { return (n + (alignment - Size::value(1))) & ~(alignment - Size::value(1)); }
			constexpr bool pointer_in_buffer(const std::byte* p) const noexcept { return buffer_ <= p && p <= buffer_ + N; }
			alignas(alignment) std::byte buffer_[N];
			std::byte* ptr_{};
		public:
			arena() noexcept : ptr_(buffer_) {}
			arena(const arena&) = delete;
			arena& operator=(const arena&) = delete;
			constexpr bool reset() noexcept { ptr_ = buffer_; }
			static constexpr Size::type size() noexcept { return N; }
			constexpr Size::type used() const noexcept { return static_cast<Size::type>(ptr_ - buffer_); }
			constexpr std::byte* allocate(Size::type n)
			{
				const auto aligned_n = align_up(n);
				const auto available_bytes = static_cast<decltype(aligned_n)>(buffer_ + N - ptr_);
				if (available_bytes >= aligned_n)
				{
					std::byte* r = ptr_;
					ptr_ += aligned_n;
					return r;
				}
				return static_cast<std::byte*>(::operator new(n));
			}
			constexpr void deallocate(std::byte* p, Size::type n) noexcept
			{
				if (pointer_in_buffer(p))
				{
					n = align_up(n);
					if (p + n == ptr_)
					{
						ptr_ = p;
					}
				}
				else
				{
					::operator delete(p);
				}
			}
		};

		/*****************************************************************************************
		*
		*                                    design patterns
		*
		*****************************************************************************************/
		// observer design pattern
		class observable;
		class observer
		{
		public:
			virtual ~observer() = default;
			virtual void update(observable&) = 0;
		};
		class observable
		{
		public:
			virtual ~observable() = default;
			inline void attach(observer& o) { observers_.push_back(&o); }
			inline void detach(observer& o) { observers_.erase(std::remove(observers_.begin(), observers_.end(), &o)); }
			inline void notify() { for (auto* o : observers_) o->update(*this); }

		private:
			std::vector<observer*> observers_;
		};

		/*****************************************************************************************
		*
		*                                     string traits
		*
		*****************************************************************************************/
		// string tokenization various implementations
		template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
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
		template<typename Enc, typename EncTraits = std::char_traits<Enc>>
		constexpr void tokenize(
			std::basic_string_view<Enc, EncTraits> query,
			std::basic_string_view<Enc, EncTraits> delimiters,
			hash_multiset<
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
		template<typename Enc, typename EncTraits = std::char_traits<Enc>>
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

		// frequencies
		template<typename Enc, typename EncTraits = std::char_traits<Enc>>
		constexpr decltype(auto) frequencies(
			std::basic_string_view<Enc, EncTraits> query,
			std::basic_string_view<Enc, EncTraits> delimiters
		)
		{
			hash_map<
				std::basic_string_view<Enc, EncTraits>,
				Size::type,
				std::hash<std::basic_string_view<Enc, EncTraits>>,
				std::equal_to<std::basic_string_view<Enc, EncTraits>>,
				std::allocator<std::pair<const std::basic_string_view<Enc, EncTraits>, Size::type>>
			> freqs{};

			// skip delimiters at begining
			auto last_pos = query.find_first_not_of(delimiters, 0);

			// find first non-delimiter
			auto pos = query.find_first_of(delimiters, last_pos);

			while (pos != std::basic_string_view<Enc, EncTraits>::npos || last_pos != std::basic_string_view<Enc, EncTraits>::npos)
			{
				// find a token, add it to the vector
				freqs[query.substr(last_pos, pos - last_pos)]++;

				// skip delimiters
				last_pos = query.find_first_not_of(delimiters, pos);

				// find next non-delimiter
				pos = query.find_first_of(delimiters, last_pos);
			}

			return freqs;
		}

		// to lowercase function
		template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
		constexpr decltype(auto) to_lowercase(const std::basic_string<Enc, EncTraits, EncAlloc>& word)
		{
			std::basic_string<Enc, EncTraits, EncAlloc> lowercased_word{};
			lowercased_word.resize(std::ranges::size(word));
			std::ranges::transform(std::ranges::cbegin(word), std::ranges::cend(word),
				std::ranges::begin(lowercased_word), [](const auto& c) { return tolower(c); });
			return lowercased_word;
		}
		template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
		constexpr decltype(auto) to_lowercase(std::basic_string_view<Enc, EncTraits> word_view)
		{
			std::basic_string<Enc, EncTraits, EncAlloc> lowercased_word{};
			lowercased_word.resize(std::ranges::size(word_view));
			std::ranges::transform(std::ranges::cbegin(word_view), std::ranges::cend(word_view),
				std::ranges::begin(lowercased_word), [](const auto& c) { return std::tolower(c); });
			return lowercased_word;
		}

		// to uppercase function
		template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
		constexpr decltype(auto) to_uppercase(const std::basic_string<Enc, EncTraits, EncAlloc>& word)
		{
			std::basic_string<Enc, EncTraits, EncAlloc> uppercased_word{};
			uppercased_word.resize(std::ranges::size(word));
			std::ranges::transform(std::ranges::cbegin(word), std::ranges::cend(word),
				std::ranges::begin(uppercased_word), [](const auto& c) { return std::toupper(c); });
			return uppercased_word;
		}
		template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
		constexpr decltype(auto) to_uppercase(std::basic_string_view<Enc, EncTraits> word_view)
		{
			std::basic_string<Enc, EncTraits, EncAlloc> uppercased_word{};
			uppercased_word.resize(std::ranges::size(word_view));
			std::ranges::transform(std::ranges::cbegin(word_view), std::ranges::cend(word_view),
				std::ranges::begin(uppercased_word), [](const auto& c) { return std::toupper(c); });
			return uppercased_word;
		}

		/*****************************************************************************************
		*
		*                                      time traits
		*
		*****************************************************************************************/
		// countdown function
		template<std::integral I> constexpr void countdown(I nsec)
		{
			std::cout << "\nT-" << nsec << ' ';
			std::this_thread::sleep_for(std::chrono::seconds(1));
			for (auto i{ signed_integer::_64bit::value(nsec) - signed_integer::_64bit::value(1) }; i >= signed_integer::_64bit::value(0); --i)
			{
				std::cout << i << ' ';
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}

		// runtime calculator
		template<typename F, typename... Args> constexpr decltype(auto) runtime(F&& f, Args&&... args)
		{
			if constexpr (std::is_void_v<std::invoke_result_t<F, Args...>>)
			{
				auto ti{ high_resolution_clock::now() };
				std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
				auto tf{ high_resolution_clock::now() };
				return duration<real::double_precision::type>(tf - ti).count();
			}
			if constexpr (!std::is_void_v<std::invoke_result_t<F, Args...>>)
			{
				auto ti{ high_resolution_clock::now() };
				auto retval{ std::invoke(std::forward<F>(f), std::forward<Args>(args)...) };
				auto tf{ high_resolution_clock::now() };
				return std::make_pair(std::move(retval), duration<real::double_precision::type>(tf - ti).count());
			}
		}

		// time traits
		template<std::integral I> struct time_frames
		{
			real::double_precision::type initial_time, final_time;
			I number_of_time_steps;
			time_frames(real::double_precision::type ti, real::double_precision::type tf, I nts) :initial_time{ ti }, final_time{ tf }, number_of_time_steps{ nts } {}
			inline dynamic_array<real::double_precision::type> generate()
			{
				auto dt{ (final_time - initial_time) / real::double_precision::value(number_of_time_steps) };
				dynamic_array<real::double_precision::type> t(0.0, number_of_time_steps);
				for (auto i : std::ranges::views::iota((I)0, number_of_time_steps))
					t[i] = initial_time + (i * dt);
				return t;
			}
			inline dynamic_array<real::double_precision::type> operator()() { return generate(); }
		};

		// wait for some times in different time units
		struct wait
		{
			class value_type
			{
			public:
				value_type() = delete;
				template<arithmetic T> value_type(T value) :value_(value) {}
				value_type(const value_type&) = delete;
				value_type(value_type&&) noexcept = delete;
				virtual ~value_type() = default;
				inline value_type& operator=(const value_type&) = delete;
				inline value_type& operator=(value_type&&) noexcept = delete;
				inline void nanoseconds() { std::this_thread::sleep_for(duration<real::double_precision::type, std::ratio<1, 1'000'000'000>>(value_)); }
				inline void microseconds() { std::this_thread::sleep_for(duration<real::double_precision::type, std::ratio<1, 1'000'000>>(value_)); }
				inline void milliseconds() { std::this_thread::sleep_for(duration<real::double_precision::type, std::ratio<1, 1'000>>(value_)); }
				inline void seconds() { std::this_thread::sleep_for(duration<real::double_precision::type, std::ratio<1>>(value_)); }
				inline void minutes() { std::this_thread::sleep_for(duration<real::double_precision::type, std::ratio<60>>(value_)); }
				inline void hours() { std::this_thread::sleep_for(duration<real::double_precision::type, std::ratio<3'600>>(value_)); }
				inline void days() { std::this_thread::sleep_for(duration<real::double_precision::type, std::ratio<86'400>>(value_)); }
			private:
				real::double_precision::type value_;
			};
			wait() {}
			wait(const wait&) = delete;
			wait(wait&&) noexcept = delete;
			virtual ~wait() = default;
			inline wait& operator=(const wait&) = delete;
			inline wait& operator=(wait&&) noexcept = delete;
			template<arithmetic T> constexpr value_type f0r(T value) const { return value_type(value); }
		};
	}
}

#endif// !__TOOLB0X_DEVELOPED_BY_POORIA_YOUSEFI_AT_8_2_2024_H_
