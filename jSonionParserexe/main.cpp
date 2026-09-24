#ifndef JSONIONING
#define JSONIONING

#include "jSonion.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <stdexcept>

//Testing area

int main() {
	jSONion::JSON test{};
	//btw tester is literally just ripped straight out of the wiki page for JSON
	test.parse("jSonionfiles/Tester.json");

	std::cout << static_cast<std::string>(test["first_name"]) << std::endl;

	return 0;
}

#endif // !JSONIONING