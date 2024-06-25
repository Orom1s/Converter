#pragma once

bool ichar_equals(char a, char b);

bool iequals(std::string_view lhs, std::string_view rhs);

std::string quotesql(const std::string& s);

std::string ReadLine(std::istream& input);

std::wstring GetWStringFromString(const std::string& str);

std::string GetStringFromWString(const std::wstring& wstr);