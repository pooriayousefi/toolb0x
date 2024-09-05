
#include "toolb0x.h"

using namespace tbx;

// tester closures namespace
namespace test
{
	static inline void filesystem_path_copy_1()
	{
		std::filesystem::path src(R"(C:\Users\Pooria\Documents\ebooks)");
		std::filesystem::path dst(R"(C:\tools)");
		std::cout << "\ncopy time: " << runtime([&]()
			{
				filesystem_path::copy_in_ascending_counted_numbers_if_the_path_already_existed()(src, dst);
			}) << " (sec)" << std::endl;
	}
}

// entry-point
int main()
{
	// empty text file generator
	auto txtgen = []()
		{
			std::filesystem::path fp{ L"New Text File.txt" };
			auto counter{ 0 };

			while (std::filesystem::exists(fp))
			{
				std::filesystem::path newfp{ native::wide_encoded::string{ L"New Text File (" } +
				std::to_wstring(++counter) +
				native::wide_encoded::string{ L").txt" } };
				std::swap(fp, newfp);
			}

			native::wide_encoded::output_file output_file{};
			output_file.open(fp);
		};
	try
	{	
#ifdef _DEBUG
		test::filesystem_path_copy_1();
#endif
		return 0;
	}
	catch (const std::exception& xxx)
	{
		std::cerr << xxx.what() << std::endl;
		return 1;
	}
}