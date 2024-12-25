
#include <iostream>
#include <exception>
#include <vector>

import concepts;
import from_to;
import iterate;
import histograms;
import shuffle_and_sample;
import countdown;
import runtime;
import time_frames;
import wait_for;
import tokenize;
import stringformers;
import file_streams;
import filesystem_paths;
import asyncgen;
import asynctask;
import misc;
import syncgen;
import result;
import meta;
import observer;
import memory;
import numerical;
import conversions;
import statistics;
import random;
import algoviews;

int main()
{
	try
	{
		for (auto i : from(0).to(23))
			std::cout << i << ' ';
		std::cout << '\n' << runtime(
			[]()
			{
				std::vector<int> candidates{ -2, 9, 0, 5, 6, 0, -4, 8, 0, 3, -2 };
				auto samples{ do_n_times_shuffle_and_sample(candidates.begin(), candidates.end(), 1000) };
				auto candidates_histogram{ histogram(samples) };
				for (auto& kv : candidates_histogram)
					std::cout << kv.first << " ---> " << kv.second << '\n';
				auto tf{ time_frames(-101.002, 13.09, 10).generate() };
				for (auto& elem : tf)
					std::cout << elem << ' ';
				std::cout << '\n';
				wait_for(3).seconds();
				std::wstring sentence{ L"ABCD EFghy" };
				std::wcout 
					<< L"\n\n"
					<< sentence << L" in lowercased form = " << to_lowercase(sentence)
					<< L"\nand in uppercased form = " << to_uppercase(sentence) << std::endl;
			}
		) << " (sec)\n";
		countdown(5);
		return 0;
	}
	catch (const std::exception& xxx)
	{
		std::cerr << xxx.what() << std::endl;
		return 1;
	}
}