export module misc;

import <utility>;
import <vector>;

export template<typename T, typename AT = std::allocator<T>, typename AVT = std::allocator<std::vector<T, AT>>>
using table = std::vector<std::vector<T, AT>, AVT>;

export template<typename T1, typename T2 = T1> using pair = std::pair<T1, T2>;
