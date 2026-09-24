#ifndef IDKWHATTONAMETHIS
#define IDKWHATTONAMETHIS

#include "jSonionExceptions.h"

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <type_traits>
#include <initializer_list>
#include <iterator>

namespace jSONion {
	struct JSONIONval;

	template <typename T>
	concept JSONIONvalType =
		std::same_as<T, std::string> ||
		std::same_as<T, double> ||
		std::same_as<T, bool> ||
		std::same_as<T, std::nullptr_t> ||
		std::same_as<T, std::vector<JSONIONval>> ||
		std::same_as<T, std::unordered_map<std::string, JSONIONval>>;

	template <typename T>
	concept JSONIONvalTypeMath = JSONIONvalType<T> || std::is_arithmetic_v<T> || std::is_same_v<T, char>;

	struct JSONIONval : std::variant<std::string, double, bool, std::nullptr_t, std::vector<JSONIONval>, std::unordered_map<std::string, JSONIONval>> {
		using std::variant<std::string, double, bool, std::nullptr_t, std::vector<JSONIONval>, std::unordered_map<std::string, JSONIONval>>::variant;

		//default rule of five
		JSONIONval(const JSONIONval&) = default;
		JSONIONval(JSONIONval&&) noexcept = default;
		JSONIONval& operator=(const JSONIONval&) = default;
		JSONIONval& operator=(JSONIONval&&) noexcept = default;
		~JSONIONval() = default;

		//constructor with initializer list
		JSONIONval(std::initializer_list<std::pair<std::string, JSONIONval>> list);//maps

		JSONIONval& operator[](const std::string& key);
		JSONIONval& operator[](std::size_t index);

		JSONIONval& operator+=(std::pair<const std::string, JSONIONval>& added);
		JSONIONval& operator+=(const JSONIONval& added);

		template <JSONIONvalType T>
		operator T() const {
			if (std::holds_alternative<T>(*this)) { return std::get<T>(*this); }
			throw JsonTypeError("Cannot convert JSON value to this type");
		}
		template <JSONIONvalTypeMath T>
		explicit operator T() const {
			if (std::holds_alternative<double>(*this) && std::is_arithmetic_v<T>) { return static_cast<T>(std::get<double>(*this)); }
			if (std::holds_alternative<std::string>(*this) && std::is_same_v<T, char>) {
				std::string convert = std::get<std::string>(*this);
				if (convert.size() == 1) { return convert[0]; }
				else { throw JsonTypeError("JSON string is to big to convert to char"); }
			}
			throw JsonTypeError("Cannot convert JSON value to this type");
		}

		template <JSONIONvalType T>
		bool is() const {
			return std::holds_alternative<T>(*this);
		}

		template <JSONIONvalTypeMath T>
		T get() const {
			if constexpr (JSONIONvalType<T>) {
				if (std::holds_alternative<T>(*this)) {
					return std::get<T>(*this);
				}
			}

			if constexpr (std::is_arithmetic_v<T>) {
				if (std::holds_alternative<double>(*this)) {
					return static_cast<T>(std::get<double>(*this));
				}
			}

			if constexpr (std::is_same_v<T, char>) {
				if (auto* str = std::get_if<std::string>(this)) {
					if ((*str).size() == 1) {
						return (*str)[0];
					}
				}
			}

			throw JsonTypeError("Could not get value");
		}

		bool is_num() const;

		bool empty() const;
		std::size_t size() const;

		//iterators for json arrays
		std::vector<JSONIONval>::iterator abegin();
		std::vector<JSONIONval>::iterator aend();

		//iterators for json objects
		std::unordered_map<std::string, JSONIONval>::iterator obegin();
		std::unordered_map<std::string, JSONIONval>::iterator oend();
	};

	enum class TokenType {
		BraceOpen,
		BraceClose,
		BracketOpen,
		BracketClose,
		String,
		Number,
		Comma,
		Colon,
		True,
		False,
		Null
	};

	struct Token {
		TokenType type;
		std::string value;

		Token(TokenType t, std::string v) : type(t), value(v) {}
	};
};

#endif // !IDKWHATTONAMETHIS