#include "stdafx.h"
#include "Test.h"
#include "Converter.h"
#include <assert.h>

using namespace std::string_literals;

int main() {
	//const std::string& file = "E:\\C++edu\\Converter\\Loa\\GOST-8240-89.loa"s;
	/*std::string path_to_directory = ReadLine(std::cin);
	ForEachFilesInDir(path_to_directory);*/
	//Converter("E:\\C++edu\\Converter\\Loa\\GOST-8509-93.loa"s);

	SpecBladingProfileParams tmp;
	SpecBladingDataLoader loader;
	if (loader.ReadGOST8509Params(tmp, L"E:\\C++edu\\Converter\\Loa\\GOST-8509-93.db")) {
		std::cout << "Its working";
	} else std::cout << "Not working";
}