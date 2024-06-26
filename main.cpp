#include "stdafx.h"
#include "Test.h"
#include "Converter.h"
#include <assert.h>
#include "Converter.h"

using namespace std::string_literals;
/*! \brief Печать справки в консоль
*/
void print_help()
{
  std::wcout << L"loaconv" << L" v0.0.1" << std::endl;
  std::wcout << "loa-database convertor" << std::endl;
  std::wcout << L"usage:" << std::endl << std::endl;
  std::wcout << L"Converter" << L" <input_dyrectory>" << std::endl;
  std::wcout << L"    <input_dyrectory>             path to find loa-database file" << std::endl;
  std::wcout << std::endl << std::flush;
}

int main(int argc, char* argv[]) {
  // Check the number of parameters
  if (argc != 2)
  {
    std::wcerr << L"invalid argument count!" << std::endl << std::endl;
    print_help();
    return EXIT_FAILURE;
  }

	std::filesystem::path execPath = argv[1];
	ForEachFilesInDir(execPath);

// 	SpecBladingProfileParams tmp;
// 	SpecBladingDataLoader loader;
// 	if (loader.ReadGOST8509Params(tmp, L"E:\\C++edu\\Converter\\Loa\\GOST-8509-93.db")) {
// 		std::cout << "Its working";
// 	} else std::cout << "Not working";
}