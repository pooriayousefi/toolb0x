export module filesystem_paths;

import <filesystem>;
import <string>;
import <stack>;
import <utility>;
import <span>;
import <ranges>;
import <algorithm>;
import <iostream>;
import <fstream>;
import file_streams;

// filesystem path class
export class filesystem_path
{
public:
	filesystem_path() :m_filepath{}
	{
	}
	filesystem_path(std::filesystem::path filepath) :m_filepath{ filepath }
	{
	}
	inline void set(std::filesystem::path filepath) { m_filepath = filepath; }

	// filesystem path serializer
	inline void serialize()
	{
		binary_string buffer{};
		buffer.reserve(static_cast<size_t>(1'073'741'824));
		auto serialize_directory = [&buffer](std::filesystem::path p)
			{
				// phase 1: watermark as a directory
				buffer.push_back(static_cast<std::byte>(true));

				// phase 2: serialize directory name size 
				auto directory_name{ p.wstring() };
				auto directory_name_size{ directory_name.size() };
				binary_string directory_name_size_subbuffer(sizeof(size_t), std::byte{});
				memcpy(directory_name_size_subbuffer.data(), &directory_name_size, sizeof(size_t));
				buffer.append(directory_name_size_subbuffer);

				// phase 3: serialize directory name itself
				binary_string directory_name_subbuffer(directory_name_size * sizeof(wchar_t), std::byte{});
				memcpy(directory_name_subbuffer.data(), directory_name.data(), directory_name_size * sizeof(wchar_t));
				buffer.append(directory_name_subbuffer);
			};
		auto serialize_regular_file = [&buffer](std::filesystem::path p)
			{
				// phase 1: watermark as a file
				buffer.push_back(static_cast<std::byte>(false));

				// phase 2: serialize file name size
				auto filename{ p.wstring() };
				auto filename_size{ filename.size() };
				binary_string filename_size_subbuffer(sizeof(size_t), std::byte{});
				memcpy(filename_size_subbuffer.data(), &filename_size, sizeof(size_t));
				buffer.append(filename_size_subbuffer);

				// phase 3: serialize file name itself
				binary_string filename_subbuffer(filename_size * sizeof(wchar_t), std::byte{});
				memcpy(filename_subbuffer.data(), filename.data(), filename_size * sizeof(wchar_t));
				buffer.append(filename_subbuffer);

				// phase 4: serialize file contents size
				auto file_contents_size{ std::filesystem::file_size(p) };
				binary_string file_contents_size_subbuffer(sizeof(size_t), std::byte{});
				memcpy(file_contents_size_subbuffer.data(), &file_contents_size, sizeof(size_t));
				buffer.append(file_contents_size_subbuffer);

				// phase 5: serialize file contents themselves
				auto file_contents_subbuffer{ binary_input_file().read(p) };
				buffer.append(file_contents_subbuffer);
			};
		std::stack<std::filesystem::path> s{};
		s.push(m_filepath);
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
		binary_output_file outputfile{};
		outputfile.stream.open(m_filepath.wstring() + std::wstring(L".serialized"));
		for (auto& e : buffer)
			outputfile.stream << e;
	}

	// filesystem path deserializer
	inline void deserialize()
	{
		if (std::filesystem::is_regular_file(m_filepath) && m_filepath.extension().string() == std::string{ ".serialized" })
		{
			auto buffer{ binary_input_file().read(m_filepath) };

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
					std::span<std::byte> directory_name_size_span(it, sizeof(size_t));
					size_t directory_name_size{};
					memcpy(&directory_name_size, directory_name_size_span.data(), directory_name_size_span.size());
					it = std::ranges::next(it, directory_name_size_span.size());

					// phase 2: directory name extraction from binary serialized file (in the size of directory_name_size)
					std::span<std::byte> directory_name_span(it, directory_name_size * sizeof(wchar_t));
					std::wstring directory_name(directory_name_size, wchar_t{});
					memcpy(directory_name.data(), directory_name_span.data(), directory_name_span.size());
					it = std::ranges::next(it, directory_name_span.size());

					// phase 3: create the directory with the above extracted name
					std::filesystem::create_directory(std::filesystem::path(directory_name));
				}
				else// if there is a file in the following
				{
					// phase 1: file name size extraction from binary serialized file
					std::span<std::byte> filename_size_span(it, sizeof(size_t));
					size_t filename_size{};
					memcpy(&filename_size, filename_size_span.data(), filename_size_span.size());
					it = std::ranges::next(it, filename_size_span.size());

					// phase 2: file name extraction from binary serialized file (in the size of filename_size)
					std::span<std::byte> filename_span(it, filename_size * sizeof(wchar_t));
					std::wstring filename(filename_size, wchar_t{});
					memcpy(filename.data(), filename_span.data(), filename_span.size());
					it = std::ranges::next(it, filename_span.size());

					// phase 3: file contents size extraction from binary serialized file
					std::span<std::byte> file_contents_size_span(it, sizeof(size_t));
					size_t file_contents_size{};
					memcpy(&file_contents_size, file_contents_size_span.data(), file_contents_size_span.size());
					it = std::ranges::next(it, file_contents_size_span.size());

					// phase 4: file contents extraction from binary serialized file (in the size of file_contents_size)
					std::span<std::byte> file_contents_span(it, file_contents_size);
					binary_output_file output_file{};
					output_file.stream.open(filename, std::ios_base::out | std::ios_base::binary);
					if (!output_file.stream.is_open())
					{
						auto errmsg =
							std::string{ "ERROR: Cannot open " } +
							std::filesystem::path(filename).string() +
							std::string{ " file." };
						throw std::runtime_error(errmsg.c_str());
					}
					std::ranges::for_each(
						file_contents_span, 
						[&output_file](auto& elem) { output_file.stream << elem; }
					);
					it = std::ranges::next(it, file_contents_span.size());
				}
			}
		}
	}

	// copt to type
	class copy_to_t
	{
	public:
		copy_to_t() :m_source_filepath{}, m_destination_filepath{}
		{
		}
		copy_to_t(std::filesystem::path source_filepath, std::filesystem::path destination_filepath) :
			m_source_filepath{ source_filepath },
			m_destination_filepath{ destination_filepath }
		{
		}

		// filesystem path copy in ascending counted numbers if the path already existed
		inline void in_ascending_counted_numbers_if_it_is_already_existed() const
		{
			auto copy_regular_file = [](std::filesystem::path source_path, std::filesystem::path destination_path)
				{
					std::filesystem::copy(source_path, [&destination_path]()
						{
							std::filesystem::path new_destination_path{ destination_path };
							const auto destination_parent_path{ destination_path.parent_path() };
							const auto destination_stem{ destination_path.stem().wstring() };
							const auto destination_extension{ destination_path.extension().wstring() };
							auto counter{ static_cast<size_t>(0) };
							while (std::filesystem::exists(new_destination_path))
							{
								new_destination_path.clear();
								new_destination_path = destination_parent_path /
									std::filesystem::path(destination_stem +
										std::wstring{ L" (" } +
										std::to_wstring(++counter) +
										std::wstring{ L")" } +
										destination_extension);
							}
							return new_destination_path;
						}());
				};
			auto copy_directory = [copy_regular_file](std::filesystem::path source_path, std::filesystem::path destination_path)
				{
					auto new_destination_path{ destination_path };
					auto counter{ static_cast<size_t>(0) };
					while (std::filesystem::exists(new_destination_path))
					{
						new_destination_path.clear();
						new_destination_path = destination_path / std::filesystem::path(source_path.filename().wstring() +
							std::wstring{ L" (" } +
							std::to_wstring(++counter) +
							std::wstring{ L")" });
					}
					std::filesystem::create_directories(new_destination_path);
					std::stack<std::pair<std::filesystem::path, std::filesystem::path>> stk{};
					for (auto& entry : std::filesystem::directory_iterator(source_path))
					{
						auto source_child_path{ entry.path() };
						auto new_destination_child_path = new_destination_path / source_child_path.filename();
						stk.push(std::make_pair(std::move(source_child_path), std::move(new_destination_child_path)));
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
								auto srcChildPath{ entry.path() };
								auto dstChildPath = dst / srcChildPath.filename();
								stk.push(std::make_pair(srcChildPath, dstChildPath));
							}
						}
						else if (std::filesystem::is_regular_file(src))
							copy_regular_file(src, dst);
					}
				};
			if (std::filesystem::is_directory(m_source_filepath))
				copy_directory(m_source_filepath, m_destination_filepath);
			else if (std::filesystem::is_regular_file(m_source_filepath))
				copy_regular_file(m_source_filepath, m_destination_filepath);
		}

	private:
		std::filesystem::path m_source_filepath, m_destination_filepath;
	};

	// filesystem path copy in ascending counted numbers if the path already existed
	inline copy_to_t copy_to(std::filesystem::path dst) const { return copy_to_t(m_filepath, dst); }

private:
	std::filesystem::path m_filepath;
};
