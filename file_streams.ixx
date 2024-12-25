export module file_streams;

import <type_traits>;
import <stdexcept>;
import <fstream>;
import <string>;
import <string_view>;
import <iostream>;
import <filesystem>;
import <vector>;
import <valarray>;
import <ranges>;

// string type aliases
export using binary_string = std::basic_string<std::byte>;
export using binary_string_view = std::basic_string_view<std::byte>;

// RAII (Resource Acquisition Is Initialization)
namespace raii
{
	template<typename Enc, typename EncTraits = std::char_traits<Enc>, typename EncAlloc = std::allocator<Enc>>
	struct basic_input_file
	{
		std::basic_ifstream<Enc, EncTraits> stream;
		basic_input_file() :stream{}
		{
		}
		virtual ~basic_input_file()
		{
			if (stream.is_open())
				stream.close();
		}
		constexpr std::basic_string<Enc, EncTraits, EncAlloc> read(
			std::filesystem::path filepath,
			std::ios_base::openmode opm = std::ios_base::in
		)
		{
			std::basic_string<Enc, EncTraits, EncAlloc> filecontents{};
			if (stream.is_open())
				stream.close();
			if constexpr (!std::is_same_v<Enc, std::byte>)
			{
				stream.open(filepath, std::ios_base::in | opm);
				if (!stream.is_open())
				{
					auto errmsg =
						std::string{ "ERROR: Cannot open " } +
						filepath.string() +
						std::string{ " file." };
					throw std::runtime_error(errmsg.c_str());
				}
			}
			else if constexpr (std::is_same_v<Enc, std::byte>)
			{
				stream.open(filepath, std::ios_base::in | std::ios_base::binary | opm);
				if (!stream.is_open())
				{
					auto errmsg =
						std::string{ "ERROR: Cannot open " } +
						filepath.string() +
						std::string{ " file." };
					throw std::runtime_error(errmsg.c_str());
				}
			}
			stream.seekg(0, std::ios_base::end);
			filecontents.reserve(stream.tellg());
			stream.seekg(0, std::ios_base::beg);
			filecontents.assign(
				std::istreambuf_iterator<Enc, EncTraits>(stream),
				std::istreambuf_iterator<Enc, EncTraits>()
			);

			return filecontents;
		}
		constexpr std::vector<std::basic_string<Enc, EncTraits, EncAlloc>> read_line_by_line(
			std::filesystem::path filepath,
			std::ios_base::openmode opm = std::ios_base::in
		)
		{
			std::vector<std::basic_string<Enc, EncTraits, EncAlloc>> filelines{};
			std::basic_string<Enc, EncTraits, EncAlloc> line{};
			if (stream.is_open())
				stream.close();
			if constexpr (!std::is_same_v<Enc, std::byte>)
			{
				stream.open(filepath, std::ios_base::in | opm);
				if (!stream.is_open())
				{
					auto errmsg =
						std::string{ "ERROR: Cannot open " } +
						filepath.string() +
						std::string{ " file." };
					throw std::runtime_error(errmsg.c_str());
				}
			}
			else if constexpr (std::is_same_v<Enc, std::byte>)
			{
				stream.open(filepath, std::ios_base::in | std::ios_base::binary | opm);
				if (!stream.is_open())
				{
					auto errmsg =
						std::string{ "ERROR: Cannot open " } +
						filepath.string() +
						std::string{ " file." };
					throw std::runtime_error(errmsg.c_str());
				}
			}

			stream.seekg(0, std::ios_base::end);
			filelines.reserve(stream.tellg());
			stream.seekg(0, std::ios_base::beg);
			while (!stream.eof())
			{
				std::getline<Enc, EncTraits, EncAlloc>(stream, line);
				filelines.emplace_back(line);
			}

			return filelines;
		}
	};
	template<typename Enc, typename EncTraits = std::char_traits<Enc>>
	struct basic_output_file
	{
		std::basic_ofstream<Enc, EncTraits> stream;
		basic_output_file() :stream{}
		{
		}
		virtual ~basic_output_file()
		{
			if (stream.is_open())
			{
				stream.flush();
				stream.close();
			}
		}
	};
}
export using native_narrow_encoded_input_file = raii::basic_input_file<char>;
export using native_wide_encoded_input_file = raii::basic_input_file<wchar_t>;
export using utf8_encoded_input_file = raii::basic_input_file<char8_t>;
export using binary_input_file = raii::basic_input_file<std::byte>;

export using native_narrow_encoded_output_file = raii::basic_output_file<char>;
export using native_wide_encoded_output_file = raii::basic_output_file<wchar_t>;
export using utf8_encoded_output_file = raii::basic_output_file<char8_t>;
export using binary_output_file = raii::basic_output_file<std::byte>;

export inline native_narrow_encoded_output_file& operator<<(native_narrow_encoded_output_file& of, const std::vector<bool>& v)
{
	for (auto i : std::ranges::views::iota((size_t)0, v.size()))
		switch (v[i])
		{
		case true:
			of.stream << '1';
			break;
		default:
			of.stream << '0';
			break;
		}
	of.stream << '\n';
	return of;
}
export inline native_narrow_encoded_output_file& operator<<(native_narrow_encoded_output_file& of, const std::valarray<bool>& v)
{
	for (auto i : std::ranges::views::iota((size_t)0, v.size()))
		switch (v[i])
		{
		case true:
			of.stream << '1';
			break;
		default:
			of.stream << '0';
			break;
		}
	of.stream << '\n';
	return of;
}
export inline native_wide_encoded_output_file& operator<<(native_wide_encoded_output_file& of, const std::vector<bool>& v)
{
	for (auto i : std::ranges::views::iota((size_t)0, v.size()))
		switch (v[i])
		{
		case true:
			of.stream << L'1';
			break;
		default:
			of.stream << L'0';
			break;
		}
	of.stream << L'\n';
	return of;
}
export inline native_wide_encoded_output_file& operator<<(native_wide_encoded_output_file& of, const std::valarray<bool>& v)
{
	for (auto i : std::ranges::views::iota((size_t)0, v.size()))
		switch (v[i])
		{
		case true:
			of.stream << L'1';
			break;
		default:
			of.stream << L'0';
			break;
		}
	of.stream << L'\n';
	return of;
}
