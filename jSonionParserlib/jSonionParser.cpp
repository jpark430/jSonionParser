#include "jSonionParser.h"
#include "jSonionExceptions.h"

using namespace jSONion;

std::string jSONion::JsonValToSTR(JSONIONval val) {
	if (std::holds_alternative<std::string>(val)) return std::get<std::string>(val);
	else if (std::holds_alternative<double>(val)) return std::to_string(std::get<double>(val));
	else if (std::holds_alternative<bool>(val)) return (std::get<bool>(val)) ? std::string{ "true" } : std::string{ "false" };
	else if (std::holds_alternative<std::nullptr_t>(val)) return std::string{ "null" };
	else return std::string{ "unknown" };
}

bool jSONion::charTest(char tested) noexcept {
	int testASCII = static_cast<int>(tested);

	return testASCII == 43 || testASCII == 45 || testASCII == 69 || (testASCII >= 48 && testASCII <= 57) || (testASCII >= 97 && testASCII <= 122);
}

bool jSONion::validNum(std::string tested) noexcept {
	std::size_t processedChars = 0;

	try {
		double result = std::stod(tested, &processedChars);
		return processedChars == tested.size();
	}
	catch (...) {
		return false;
	}
}

bool jSONion::validTrue(std::string tested) noexcept {return tested == "true";}

bool jSONion::validFalse(std::string tested) noexcept {return tested == "false";}

bool jSONion::validNull(std::string tested) noexcept {return tested == "null";}

std::string jSONion::ProgressToSTR(MapProgress Progress) {
	switch (Progress) {
	case (MapProgress::Completed):
		return std::string {"Completed"};
	case (MapProgress::Key):
		return std::string{ "Key" };
	case (MapProgress::Colon):
		return std::string{ "Colon" };
	case (MapProgress::Value):
		return std::string{ "Value" };
	}
}

std::string jSONion::ProgressToSTR(ArrayProgress Progress) {
	switch (Progress) {
	case (ArrayProgress::Completed):
		return std::string{ "Completed" };
	case (ArrayProgress::Value):
		return std::string{ "Value" };
	}
}

void jSONion::MapPart::add(Token addedToken) {
	switch (Progress) {
	case (MapProgress::Key):
		if (addedToken.type == TokenType::Colon) Progress = MapProgress::Colon;
		else throw JsonSyntaxError("Did not add a Colon on key-value pair. Added: " + addedToken.value);
		break;
	case (MapProgress::Value):
		if (addedToken.type == TokenType::Comma) Progress = MapProgress::Completed;
		else throw JsonSyntaxError("Did not add a Comma on non-ending key-value pair. Added: " + addedToken.value);
		break;
	default:
		throw JsonSyntaxError("unknown step while adding token to key-value pair: " + ProgressToSTR(Progress) + ", " + addedToken.value);
	}
}

void jSONion::MapPart::add(JSONIONval addedOnion) {
	switch (Progress) {
	case (MapProgress::Completed):
		if (std::holds_alternative<std::string>(addedOnion)) {
			JSONkey = std::get<std::string>(addedOnion);
			Progress = MapProgress::Key;
		}
		else throw JsonSyntaxError("non-string key added");
		break;
	case (MapProgress::Colon):
		JSONval = addedOnion;
		Progress = MapProgress::Value;
		break;
	default:
		throw JsonSyntaxError("Added a parsed token on wrong step of key-value pair: " + ProgressToSTR(Progress) + ", " + JsonValToSTR(addedOnion));
	}
}

void jSONion::ArrayPart::add(Token addedToken) {
	if (Progress == ArrayProgress::Value && addedToken.type == TokenType::Comma) Progress = ArrayProgress::Completed;
	else throw JsonSyntaxError("Added a token on wrong step of array element: " + addedToken.value);
}

void jSONion::ArrayPart::add(JSONIONval addedOnion) {
	if (Progress == ArrayProgress::Completed) {
		JSONval = addedOnion;
		Progress = ArrayProgress::Value;
	}
	else throw JsonSyntaxError("Added a parsed token on wrong step of array element: " + ProgressToSTR(Progress) + ", " + JsonValToSTR(addedOnion));
}

std::vector<Token> jSONion::Tokenizer(std::string jsonSTR) {
	std::vector<Token> Tokens = {};

	std::size_t Current = 0;
	std::size_t FileLength = jsonSTR.size();

	while (Current < FileLength) {
		char CurrChar = jsonSTR[Current];

		if (CurrChar == '{') {
			Tokens.emplace_back(TokenType::BraceOpen, std::string{ CurrChar });
			Current++;
			continue;
		}

		if (CurrChar == '}') {
			Tokens.emplace_back(TokenType::BraceClose, std::string{ CurrChar });
			Current++;
			continue;
		}

		if (CurrChar == '[') {
			Tokens.emplace_back(TokenType::BracketOpen, std::string{ CurrChar });
			Current++;
			continue;
		}

		if (CurrChar == ']') {
			Tokens.emplace_back(TokenType::BracketClose, std::string{ CurrChar });
			Current++;
			continue;
		}

		if (CurrChar == ':') {
			Tokens.emplace_back(TokenType::Colon, std::string{ CurrChar });
			Current++;
			continue;
		}

		if (CurrChar == ',') {
			Tokens.emplace_back(TokenType::Comma, std::string{ CurrChar });
			Current++;
			continue;
		}

		if (CurrChar == '"') {
			std::string value = "";

			CurrChar = jsonSTR[++Current];

			while (CurrChar != '"' && Current < FileLength) {
				//Check if we find a back slash
				
				if (static_cast<int>(CurrChar) == '\\') {
					CurrChar = jsonSTR[++Current];
					if (CurrChar == 'n') value += '\n';
					else if (CurrChar == '\\') value += '\\';
					else if (CurrChar == '"') value += '"';

					CurrChar = jsonSTR[++Current];
				}
				else {
					value += CurrChar;
					CurrChar = jsonSTR[++Current];
				}
			}

			Tokens.emplace_back(TokenType::String, value);
			Current++;
			continue;
		}

		if (charTest(CurrChar)) {
			std::string value = "";

			while (charTest(CurrChar) && Current < FileLength) {
				value += CurrChar;
				CurrChar = jsonSTR[++Current];
			}

			if (validNum(value)) Tokens.emplace_back(TokenType::Number, value);
			else if (validTrue(value)) Tokens.emplace_back(TokenType::True, value);
			else if (validFalse(value)) Tokens.emplace_back(TokenType::False, value);
			else if (validNull(value)) Tokens.emplace_back(TokenType::Null, value);
			continue;
		}

		if (std::isspace(static_cast<unsigned char>(CurrChar)) || CurrChar == ' ') {
			Current++;
			continue;
		}

		throw JsonSyntaxError("Unknown character in json file while tokenizing: " + std::string{ CurrChar });
	}

	return Tokens;
}

JSONIONval jSONion::JsonParser::Parse_Token() {
	JSONIONval returnedVal;

	auto& JSONToken = m_Tokens[m_current];
	switch (JSONToken.type) {
	case TokenType::BraceOpen:
		m_TheOnionStack.push(TokenType::BraceOpen);
		if (m_TheOnionStack.size() > MAX_PARSE_DEPTH) throw JsonRecursionError("reached passed max depth");
		returnedVal = Parse_Object();
		break;
	case TokenType::BracketOpen:
		m_TheOnionStack.push(TokenType::BracketOpen);
		if (m_TheOnionStack.size() > MAX_PARSE_DEPTH) throw JsonRecursionError("reached passed max depth");
		returnedVal = Parse_Array();
		break;
	case TokenType::String:
		returnedVal = JSONToken.value;
		break;
	case TokenType::Number:
		//should be no issues as we checked before when initializing token
		returnedVal = std::stod(JSONToken.value);
		break;
	case TokenType::True:
		returnedVal = true;
		break;
	case TokenType::False:
		returnedVal = false;
		break;
	case TokenType::Null:
		returnedVal = nullptr;
		break;
	default:
		throw JsonSyntaxError("unparsable token: " + JSONToken.value);
	}

	return returnedVal;
}

std::unordered_map<std::string, JSONIONval> jSONion::JsonParser::Parse_Object() {
	std::unordered_map<std::string, JSONIONval> returnedMap;
	MapPart Mapping;
	auto& currToken = m_Tokens[++m_current];

	while (currToken.type != TokenType::BraceClose) {
		if (currToken.type == TokenType::Colon || currToken.type == TokenType::Comma) Mapping.add(currToken);
		else Mapping.add(Parse_Token());
		
		if (Mapping.Progress == MapProgress::Completed) { 
			auto [it, success] = returnedMap.insert({ Mapping.JSONkey, Mapping.JSONval });
			if (!success) throw JsonKeyError("tried to add duplicate key: " + Mapping.JSONkey);
			Mapping.JSONkey = "";
			Mapping.JSONval = nullptr;
		}

		currToken = m_Tokens[++m_current];
	}

	if (Mapping.Progress == MapProgress::Value) {
		auto [it, success] = returnedMap.insert({ Mapping.JSONkey, Mapping.JSONval });
		if (!success) throw JsonKeyError("tried to add duplicate key: " + Mapping.JSONkey);
	}
	else throw JsonSyntaxError("Ending key-value pair is not properly completed");

	m_TheOnionStack.pop();
	return returnedMap;
}

std::vector<JSONIONval> jSONion::JsonParser::Parse_Array() {
	std::vector<JSONIONval> returnedArray;
	ArrayPart ArrayElement;
	auto& currToken = m_Tokens[++m_current];

	while (currToken.type != TokenType::BracketClose) {
		if (currToken.type == TokenType::Comma) ArrayElement.add(currToken);
		else ArrayElement.add(Parse_Token());

		if (ArrayElement.Progress == ArrayProgress::Completed) {
			returnedArray.push_back(ArrayElement.JSONval);
			ArrayElement.JSONval = nullptr;
		}

		currToken = m_Tokens[++m_current];
	}

	if (ArrayElement.Progress == ArrayProgress::Value) {
		returnedArray.push_back(ArrayElement.JSONval);
	}
	else throw JsonSyntaxError("Ending array element is not properly completed");

	m_TheOnionStack.pop();
	return returnedArray;
}

JSONIONval jSONion::JsonParser::Parse() {
	return Parse_Token();
}