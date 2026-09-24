#ifndef SONION
#define SONION

#include "jSonionStuff.h"
#include "jSonionParser.h"
#include "jSonionSerializer.h"
#include "jSonionExceptions.h"

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

namespace jSONion {

	class JSON {
	public:
		//default rule of five
		JSON(const JSON&) = default;
		JSON(JSON&&) noexcept = default;
		JSON& operator=(const JSON&) = default;
		JSON& operator=(JSON&&) noexcept = default;
		~JSON() = default;

		JSON(const std::string& filename);
		template <JSONIONvalType T>
		JSON(T& val) : mValue(JSONIONval(val)) {}

		//constructor with initializer list
		JSON(std::initializer_list<std::pair<std::string, JSONIONval>> list);//maps

		JSONIONval& operator[](const std::string& key);
		JSONIONval& operator[](std::size_t index);

		JSON& operator+=(std::pair<const std::string, JSONIONval>& added);
		JSON& operator+=(const JSONIONval& added);

		template <JSONIONvalType T>
		operator T() const;
		template <JSONIONvalTypeMath T>
		explicit operator T() const;

		template <JSONIONvalTypeMath T>
		T get() const;
		template <JSONIONvalType T>
		bool is() const;

		bool is_num() const;

		bool empty() const;
		std::size_t size() const;

		std::vector<JSONIONval>::iterator abegin();
		std::vector<JSONIONval>::iterator aend();

		std::unordered_map<std::string, JSONIONval>::iterator obegin();
		std::unordered_map<std::string, JSONIONval>::iterator oend();

		void parse(const std::string& filename);
		//dumping requires an object/map
		void dump(const std::string& filename, int level);
	private:
		JSONIONval mValue = nullptr;
	};
};

#endif // !SONION