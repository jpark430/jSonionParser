#include "jSonionSerializer.h"
#include "jSonionExceptions.h"

using namespace jSONion;

std::string jSONion::multiplySTR(const std::string& victim, int multi) noexcept{
	if (victim == "" || multi <= 0) return "";
	std::string returned = "";
	returned.reserve(victim.size() * multi);

	for (std::size_t i = 0; i < multi; i++) returned += victim;

	return returned;
}

std::string jSONion::JsonSerializer::JSONvalSerialize(JSONIONval val) {
	return std::visit(JSONIONoverload{
		[](std::string& s) { return "\"" + s + "\""; },
		[](double d) { return std::format("{}", d); },
		[](bool b) { return (b) ? std::string{ "true" } : std::string{ "false" }; },
		[](std::nullptr_t n) { return std::string { "null" }; },
		[this](std::vector<JSONIONval> a) { return this->JSONArraySerialize(a); },
		[this](std::unordered_map<std::string, JSONIONval> o) { return this->JSONObjectSerialize(o); }
	}, val);
}

std::string jSONion::JsonSerializer::JSONObjectSerialize(const std::unordered_map<std::string, JSONIONval>& object) {
	if (++m_depth > MAX_SERIALIZE_DEPTH) throw JsonRecursionError("reached passed max depth");
	std::string returned = "{";
	std::string trueIndent = multiplySTR(m_jsonIndentation, m_depth + 1);

	for (auto& [key, value] : object) {
		returned += m_jsonNewLine + trueIndent + "\"" + key + "\"" + ":" + m_jsonColonSpacing + JSONvalSerialize(value) + "," ;
	}

	if (returned.size() > 1) {
		returned.pop_back();
	}

	returned += m_jsonNewLine + multiplySTR(m_jsonIndentation, m_depth) + "}";
	m_depth--;
	return returned;
}

std::string jSONion::JsonSerializer::JSONArraySerialize(const std::vector<JSONIONval>& object) {
	if (++m_depth > MAX_SERIALIZE_DEPTH) throw JsonRecursionError("reached passed max depth");
	std::string returned = "[";
	std::string trueIndent = multiplySTR(m_jsonIndentation, m_depth + 1);

	for (auto& value : object) {
		returned += m_jsonNewLine + trueIndent + JSONvalSerialize(value) + ",";
	}

	if (returned.size() > 1) {
		returned.pop_back();
	}

	returned += m_jsonNewLine + multiplySTR(m_jsonIndentation, m_depth) + "]";
	m_depth--;
	return returned;
}

jSONion::JsonSerializer::JsonSerializer(int level) {
	std::string space = " "; //apparently can't use string constructor with space using brace initialization
	m_jsonIndentation = multiplySTR(space, level);
	m_jsonNewLine = (level > 0) ? "\n" : "";
	m_jsonColonSpacing = (level > 0) ? " " : "";
}

std::string jSONion::JsonSerializer::Serialize(const std::unordered_map<std::string, JSONIONval>& object) {
	return JSONObjectSerialize(object);
}