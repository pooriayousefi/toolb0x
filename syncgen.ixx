export module syncgen;

import <cstdint>;
import <filesystem>;
import <vector>;
import <iterator>;
import <stack>;
import <utility>;
import <algorithm>;
import <ranges>;
import <execution>;
import file_streams;
import asyncgen;
import misc;
import from_to;

// combinatorial tree node type
export struct combinatorial_tree_node
{
	int8_t index;
	bool state;
	combinatorial_tree_node* left;
	combinatorial_tree_node* right;
	combinatorial_tree_node()
		:index{ static_cast<int8_t>(-1) },
		state{ false },
		left{ nullptr },
		right{ nullptr }
	{
	}
};

// multi-task/multi-functional synchronous eager generator
export class generate
{
	// empty text file generator
	inline void empty_text_file()
	{
		std::filesystem::path fp{ L"New Text File.txt" };
		auto counter{ 0 };

		while (std::filesystem::exists(fp))
		{
			std::filesystem::path newfp{ std::wstring{ L"New Text File (" } +
			std::to_wstring(++counter) +
			std::wstring{ L").txt" } };
			std::swap(fp, newfp);
		}

		native_wide_encoded_output_file outputfile{};
		outputfile.stream.open(fp, std::ios_base::out);
	}

	// implementing all of the combinations of a range elements
	template<std::input_or_output_iterator It> constexpr decltype(auto) combinations(It begin, It end)
	{
		auto combinatorial_tree_node_generator = []() -> asynchronous::generator<combinatorial_tree_node*>
			{
				std::vector<combinatorial_tree_node> combinatorial_tree_nodes{};
				combinatorial_tree_nodes.reserve(static_cast<size_t>(1'024));
				while (true)
				{
					combinatorial_tree_nodes.emplace_back(combinatorial_tree_node{});
					co_yield &combinatorial_tree_nodes.back();
				}
			};

		auto combinatorial_tree_node_store = combinatorial_tree_node_generator();

		// type alias
		using T = std::iter_value_t<It>;
		using ptr = T*;

		// variables
		auto number_of_indexes{ static_cast<int8_t>(std::ranges::distance(begin, end)) };

		// build combinarorial tree
		combinatorial_tree_node_store.resume();
		auto root = combinatorial_tree_node_store.get_value();
		std::stack<combinatorial_tree_node*> q{};
		q.push(root);
		while (!q.empty())
		{
			auto r = q.top();
			if (r->index < number_of_indexes - static_cast<int8_t>(1))
			{
				q.pop();
				combinatorial_tree_node_store.resume();
				r->right = combinatorial_tree_node_store.get_value();
				r->right->index = r->index + static_cast<int8_t>(1);
				r->right->state = true;
				q.push(r->right);
				combinatorial_tree_node_store.resume();
				r->left = combinatorial_tree_node_store.get_value();
				r->left->index = r->index + static_cast<int8_t>(1);
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
				auto s = static_cast<size_t>(1);
				for (auto i : from(0).to(number_of_indexes))
					s *= static_cast<size_t>(2);
				return s;
			}());
		std::vector<bool> buffer(static_cast<size_t>(number_of_indexes), false);
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
			[begin](const std::vector<bool>& ith_bit_pattern)
			{
				auto m{ ith_bit_pattern.size() };
				std::vector<ptr> ith_combination{};
				ith_combination.reserve(m);
				for (auto j : from(0).to(m))
				{
					if (ith_bit_pattern[j])
						ith_combination.emplace_back(&(*std::ranges::next(begin, j)));
				}
				return ith_combination;
			}
		);
		return std::make_pair(std::move(all_combinations), std::move(bit_patterns));
	}
};
