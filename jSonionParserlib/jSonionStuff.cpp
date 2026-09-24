#include "jSonionStuff.h"

using namespace jSONion;

jSONion::JSONIONval::JSONIONval(std::initializer_list<std::pair<std::string, JSONIONval>> list) {
    std::unordered_map<std::string, JSONIONval> returned{};
    for (auto& pair : list) { returned.insert(pair); }
    *this = returned;
}

JSONIONval& jSONion::JSONIONval::operator[](const std::string& key) {
    if (auto* map = std::get_if<std::unordered_map<std::string, JSONIONval>>(this)) { return (*map)[key]; }
    throw JsonTypeError("You tried to key something thats not a map. KEY: " + key);
}

JSONIONval& jSONion::JSONIONval::operator[](std::size_t index) {
    if (auto* vec = std::get_if<std::vector<JSONIONval>>(this)) { return (*vec)[index]; }

    throw JsonTypeError("You tried to index something thats not an string/vector. INDEX: " + index);
}

JSONIONval& jSONion::JSONIONval::operator+=(std::pair<const std::string, JSONIONval>& added) {
    if (auto* map = std::get_if<std::unordered_map<std::string, JSONIONval>>(this)) { (*map).emplace(added); }
    else { throw JsonValueError("You tried to add a pair to something that is not a map. ADDED KEY: " + added.first); }
    return *this;
}

JSONIONval& jSONion::JSONIONval::operator+=(const JSONIONval& added) {
    if (auto* vec = std::get_if<std::vector<JSONIONval>>(this)) { (*vec).push_back(added); }
    else { throw JsonValueError("Tried to add to something that is not a vector"); }
    return *this;
}

bool jSONion::JSONIONval::is_num() const {
    if (std::holds_alternative<double>(*this)) { return true; }
    return false;
}

bool jSONion::JSONIONval::empty() const {
    if (std::holds_alternative<std::unordered_map<std::string, JSONIONval>>(*this)) { return std::get<std::unordered_map<std::string, JSONIONval>>(*this).empty(); }
    if (std::holds_alternative<std::vector<JSONIONval>>(*this)) { return std::get<std::vector<JSONIONval>>(*this).empty(); }
    throw JsonTypeError("Cannot use empty on non-container types");
}

std::size_t jSONion::JSONIONval::size() const {
    if (std::holds_alternative<std::unordered_map<std::string, JSONIONval>>(*this)) { return std::get<std::unordered_map<std::string, JSONIONval>>(*this).size(); }
    if (std::holds_alternative<std::vector<JSONIONval>>(*this)) { return std::get<std::vector<JSONIONval>>(*this).size(); }
    throw JsonTypeError("Cannot use size on non-container types");
}

std::vector<JSONIONval>::iterator jSONion::JSONIONval::abegin() {
    if (auto* vec = std::get_if<std::vector<JSONIONval>>(this)) { return (*vec).begin(); }
    throw JsonTypeError("Cannot get an array begin iterator on a non vector json value");
}

std::vector<JSONIONval>::iterator jSONion::JSONIONval::aend() {
    if (auto* vec = std::get_if<std::vector<JSONIONval>>(this)) { return (*vec).end(); }
    throw JsonTypeError("Cannot get an array end iterator on a non vector json value");
}

std::unordered_map<std::string, JSONIONval>::iterator jSONion::JSONIONval::obegin() {
    if (auto* map = std::get_if<std::unordered_map<std::string, JSONIONval>>(this)) { return (*map).begin(); }
    throw JsonTypeError("Cannot get an object begin iterator on a non map json value");
}

std::unordered_map<std::string, JSONIONval>::iterator jSONion::JSONIONval::oend() {
    if (auto* map = std::get_if<std::unordered_map<std::string, JSONIONval>>(this)) { return (*map).end(); }
    throw JsonTypeError("Cannot get an object end iterator on a non map json value");
}