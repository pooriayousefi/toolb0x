export module algoviews;

import <type_traits>;
import <iterator>;
import <set>;
import <ranges>;
import <algorithm>;
import <unordered_map>;
import <vector>;
import <optional>;
import <concepts>;
import <span>;
import asyncgen;

/*****************************************************************************************
*
*                                  algorithmic views
*
*****************************************************************************************/
template<std::input_or_output_iterator Iterator> struct less
{
	constexpr bool operator()(const Iterator& lhs, const Iterator& rhs) const
	{
		return *lhs < *rhs;
	}
};

template<std::input_or_output_iterator Iterator> struct greater
{
	constexpr bool operator()(const Iterator& lhs, const Iterator& rhs) const
	{
		return *lhs > *rhs;
	}
};

template<std::input_or_output_iterator Iterator> class ascending_sorted_view
{
public:
	using iterator_type = Iterator;
	using value_type = std::iter_value_t<iterator_type>;
	ascending_sorted_view() :_{}
	{
	}
	ascending_sorted_view(iterator_type first, iterator_type excluded_last) :_{}
	{
		for (auto it = first; it != excluded_last; it = std::ranges::next(it))
			_.emplace(it);
	}
	constexpr decltype(auto) begin() { return _.begin(); }
	constexpr decltype(auto) end() { return _.end(); }
	constexpr decltype(auto) cbegin() const { return _.cbegin(); }
	constexpr decltype(auto) cend() const { return _.cend(); }
private:
	std::multiset<iterator_type, less<iterator_type>, std::allocator<iterator_type>> _;
};

template<std::input_or_output_iterator Iterator> class discending_sorted_view
{
public:
	using iterator_type = Iterator;
	using value_type = std::iter_value_t<iterator_type>;
	discending_sorted_view() :_{}
	{
	}
	discending_sorted_view(iterator_type first, iterator_type excluded_last) :_{}
	{
		for (auto it = first; it != excluded_last; it = std::ranges::next(it))
			_.emplace(it);
	}
	constexpr decltype(auto) begin() { return _.begin(); }
	constexpr decltype(auto) end() { return _.end(); }
	constexpr decltype(auto) cbegin() const { return _.cbegin(); }
	constexpr decltype(auto) cend() const { return _.cend(); }
private:
	std::multiset<iterator_type, greater<iterator_type>, std::allocator<iterator_type>> _;
};

template<std::input_or_output_iterator Iterator> class search_view
{
public:
	using iterator_type = Iterator;
	using value_type = std::iter_value_t<iterator_type>;
	search_view() :_{}
	{
	}
	search_view(iterator_type first, iterator_type excluded_last) :_{}
	{
		for (auto it = first; it != excluded_last; it = std::ranges::next(it))
			_[*it].emplace_back(it);
	}
	constexpr decltype(auto) find(const value_type& query)
	{
		std::optional<std::reference_wrapper<std::vector<iterator_type>>> result{};
		auto it{ _.find(query) };
		if (it != _.end())
			result = std::ref(std::get<1>(*it));
		return result;
	}
	constexpr decltype(auto) begin() { return _.begin(); }
	constexpr decltype(auto) end() { return _.end(); }
	constexpr decltype(auto) cbegin() const { return _.cbegin(); }
	constexpr decltype(auto) cend() const { return _.cend(); }
private:
	std::unordered_map<value_type, std::vector<iterator_type>> _;
};

/*****************************************************************************************
*
*                     matrix-view + asynchronous (coroutine) traits
*
*****************************************************************************************/
template<std::ranges::random_access_range Rng> struct matrix_view
{
	using value_type = std::ranges::range_value_t<Rng>;
	using view_type = std::span<std::reference_wrapper<value_type>>;

	size_t number_of_rows, number_of_columns;
	view_type view;

	template<std::integral I> matrix_view(Rng& rng, I nrows, I ncols) :
		view(rng),
		number_of_rows(nrows),
		number_of_columns(ncols)
	{
	}

	template<std::integral I> constexpr value_type& operator()(I nr, I nc)
	{
		return view[(size_t)nr * number_of_columns + (size_t)nc];
	}

	template<std::integral I> inline asynchronous::generator<std::reference_wrapper<value_type>> row(I i)
	{
		auto c{ (size_t)i * number_of_columns };
		for (auto j : std::ranges::views::iota((size_t)0, number_of_columns))
			co_yield std::ref(view[c + j]);
	}

	template<std::integral I> inline asynchronous::generator<std::reference_wrapper<value_type>> column(I j)
	{
		size_t n(0), i(j);
		while (n < number_of_rows)
		{
			co_yield std::ref(view[i]);
			n++;
			i += number_of_columns;
		}
	}
};
