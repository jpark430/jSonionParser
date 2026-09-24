#include "jSonion.h"

using namespace jSONion;

jSONion::JSON::JSON(const std::string& filename) { parse(filename); }

jSONion::JSON::JSON(std::initializer_list<std::pair<std::string, JSONIONval>> list) { mValue = JSONIONval(list); }

JSONIONval& jSONion::JSON::operator[](const std::string& key) { return mValue[key]; }

JSONIONval& jSONion::JSON::operator[](std::size_t index) { return mValue[index]; }

JSON& jSONion::JSON::operator+=(std::pair<const std::string, JSONIONval>& added) {
    mValue += added;
    return *this;
}

JSON& jSONion::JSON::operator+=(const JSONIONval& added) {
    mValue += added;
    return *this;
}

template <JSONIONvalType T>
jSONion::JSON::operator T() const { return static_cast<T>(mValue); }
template <JSONIONvalTypeMath T>
jSONion::JSON::operator T() const { return static_cast<T>(mValue); }

template <JSONIONvalTypeMath T>
T jSONion::JSON::get() const { return mValue.get<T>(); }
template <JSONIONvalType T>
bool jSONion::JSON::is() const { return mValue.is<T>(); }
bool jSONion::JSON::is_num() const { return mValue.is_num(); }
bool jSONion::JSON::empty() const { return mValue.empty(); }
std::size_t jSONion::JSON::size() const { return mValue.size(); }

std::vector<JSONIONval>::iterator jSONion::JSON::abegin() { return mValue.abegin(); }
std::vector<JSONIONval>::iterator jSONion::JSON::aend() { return mValue.aend(); }
std::unordered_map<std::string, JSONIONval>::iterator jSONion::JSON::obegin() { return mValue.obegin(); }
std::unordered_map<std::string, JSONIONval>::iterator jSONion::JSON::oend() { return mValue.oend(); }

void jSONion::JSON::parse(const std::string& filename) {
	std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "SON, THERE IS NO FILE" << std::endl;
        throw 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    std::string fileString = buffer.str();

    JsonParser SonionParser(std::move(Tokenizer(fileString)));

    mValue = SonionParser.Parse();
}

void jSONion::JSON::dump(const std::string& filename, int level) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "SON, THERE IS NO FILE" << std::endl;
        throw 1;
    }

    if (std::holds_alternative<std::unordered_map<std::string, JSONIONval>>(mValue)) {
        JsonSerializer SonionSerializer(level);
        
        file << SonionSerializer.Serialize(std::get<std::unordered_map<std::string, JSONIONval>>(mValue));
    }

    else { throw JsonTypeError("Sorry we don't support dumping for non object types yet"); }

    file.close();
}