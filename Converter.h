#pragma once

#include "sqlite/sqlite3.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <string_view>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <cctype>

#pragma warning (disable : 26495)



/// ������� Callback ��� ��������� ������ � ����
using CallBackFunction = int (*)(void*, int, char**, char**);

class Converter {
public:

	Converter() = default;

	Converter(const std::string& path);


/// ������
private:
	sqlite3* m_connection; ///< ����������
	std::string m_dbPath;  ///< ���� � ���� ������, ����� �������� ����������� ���� ����������
	std::string name_db;
	
	enum Type {
		NULL_,
		TEXT,
		INT,
		DOUBLE
	};

	struct ColumnSQL {
		std::string name;
		std::string type;
		std::vector<std::string> rows;
	};

	std::vector<ColumnSQL> table_;

///������ � ����� ������
public:
	
	/// �������� ���������� � ����� ������
	static Converter* GetDBConnection();
	/// �������� ���� � ���� ������
	std::string GetDBPath();

private:

	/// ������� ���� ������ � ������ loa �����
	bool CreateDBfile();
	bool Create();
	/// ������ ����������
	bool Open();
	/// ������� ����������
	bool Close();

	bool FillDBFile();

/// ������ � �������
private:
	void Parse(std::ifstream file);
	/// ������� ���������� ������ �� ������ �� �������
	std::vector<std::string> SplitIntoWords(const std::string& text);

	Type FoundType(std::string_view str);

	std::vector<std::string> FoundTypeForCol(const std::vector<std::string>& row);

	bool FoundDuplicate(std::vector<std::string>& cols);
};

bool ichar_equals(char a, char b);

bool iequals(std::string_view lhs, std::string_view rhs);

std::string quotesql(const std::string& s);

std::string ReadLine(std::istream& input);

bool ForEachFilesInDir(std::string path_to_directory);