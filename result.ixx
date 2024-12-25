export module result;

import <type_traits>;
import <concepts>;
import <optional>;
import <variant>;
import <string>;

// error type
export using error_type = std::optional<std::string>;

// result data structure
export template<class T> struct result_type { std::variant<error_type, T> value; };
export template<> struct result_type<void> { error_type value; };

// resulting function concept
export template<class F, class R, class... Args>
concept resulting_function = std::invocable<F, Args...>&& std::is_same_v<std::invoke_result_t<F, Args...>, result_type<R>>;
