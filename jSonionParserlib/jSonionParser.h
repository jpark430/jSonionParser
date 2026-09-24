#ifndef PARSONS
#define PARSONS

#include "jSonionStuff.h"

#include <cstddef>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <stack>
#include <iostream>

const int MAX_PARSE_DEPTH = 10;

namespace jSONion {
	std::string JsonValToSTR(JSONIONval val);

	enum class MapProgress {
		Key,
		Colon,
		Value,
		Completed
	};

	std::string ProgressToSTR(MapProgress Progress);

	struct MapPart {
		std::string JSONkey{ "" };
		JSONIONval JSONval{ nullptr };

		MapProgress Progress{ MapProgress::Completed };

		void add(Token addedToken);
		void add(JSONIONval addedOnion);
	};

	enum class ArrayProgress {
		Value,
		Completed
	};

	std::string ProgressToSTR(ArrayProgress Progress);

	struct ArrayPart {
		JSONIONval JSONval{ nullptr };

		ArrayProgress Progress{ ArrayProgress::Completed };

		void add(Token addedToken);
		void add(JSONIONval addedOnion);
	};

	[[nodiscard]] bool validNum(std::string tested) noexcept;
	[[nodiscard]] bool validTrue(std::string tested) noexcept;
	[[nodiscard]] bool validFalse(std::string tested) noexcept;
	[[nodiscard]] bool validNull(std::string tested) noexcept;

	//tests if char is +, -, E, lowercase alpha, or digit
	//these appear on null, bools, and nums
	[[nodiscard]] bool charTest(char tested) noexcept;

	std::vector<Token> Tokenizer(std::string jsonSTR);

	class JsonParser final {
	public:
		JsonParser(std::vector<Token>&& movedTokens) : m_Tokens(std::move(movedTokens)) {}

		JSONIONval Parse();
	private:
		std::size_t m_current = 0;
		std::stack<TokenType> m_TheOnionStack = {};
		std::vector<Token> m_Tokens;

		JSONIONval Parse_Token();
		std::unordered_map<std::string, JSONIONval> Parse_Object();
		std::vector<JSONIONval> Parse_Array();
	};
};

#endif // !PARSONS