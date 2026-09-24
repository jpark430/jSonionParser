#ifndef NO_MORE_SON_JOKES
#define NO_MORE_SON_JOKES

#include "jSonionStuff.h"
#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include <format>

const int MAX_SERIALIZE_DEPTH = 10;

namespace jSONion {
	//overload pattern for std::visit
	//very vibe coded
	template<class... Sons> struct JSONIONoverload : Sons... { using Sons::operator()...; };
	template<class... Sons> JSONIONoverload(Sons...) -> JSONIONoverload<Sons...>;

	
	std::string multiplySTR(const std::string& victim, int multi) noexcept;

	class JsonSerializer final {
	public:
		JsonSerializer(int level);

		std::string Serialize(const std::unordered_map<std::string, JSONIONval>& object);
	private:
		std::string m_jsonIndentation;
		std::string m_jsonNewLine;
		std::string m_jsonColonSpacing;

		int m_depth = -1;

		std::string JSONvalSerialize(JSONIONval val);
		std::string JSONObjectSerialize(const std::unordered_map<std::string, JSONIONval>& object);
		std::string JSONArraySerialize(const std::vector<JSONIONval>& object);
	};
}

#endif // !NO_MORE_SON_JOKES