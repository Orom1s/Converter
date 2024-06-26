#include "stdafx.h"
#include "common.h"

bool ichar_equals(char a, char b)
{
	return std::tolower(static_cast<unsigned char>(a)) ==
		std::tolower(static_cast<unsigned char>(b));
}

bool iequals(std::string_view lhs, std::string_view rhs)
{
	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), ichar_equals);
}



std::string quotesql(const std::string& s) {
	return std::string("\"") + s + std::string("\"");
}

std::string ReadLine(std::istream& input) {
	std::string s;
	std::getline(input, s);
	return s;
}



//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::string в std::wstring
*/
//---
std::wstring GetWStringFromString(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::wstring в std::string
*/
//---
std::string GetStringFromWString(const std::wstring& wstr)
{
	/*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr.c_str());*/
	setlocale(LC_ALL, "Russian");
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}