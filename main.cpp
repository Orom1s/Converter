#include "Converter.h"

#include <assert.h>

using namespace std::string_literals;

int main() {
	//const std::string& file = "E:\\C++edu\\Converter\\Loa\\GOST-8239-89.loa"s;
	Converter("E:\\C++edu\\Converter\\Loa\\GOST-8239-89.loa"s);
	if (std::filesystem::exists(std::filesystem::path{ "E:\\C++edu\\Converter\\Loa\\GOST-8239-89.db"s })) {
		std::cerr << "goooood";
	} else std::cerr << "fail";
}