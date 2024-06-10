#pragma once

#include "sqlite/sqlite3.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <string_view>
#include <iostream>

#pragma warning (disable : 26495)


using Var = std::variant<std::nullptr_t, std::string, int, double>;

/// ������� Callback ��� ��������� ������ � ����
using CallBackFunction = int (*)(void*, int, char**, char**);

class Converter {
	friend std::vector<Var> CheckSplit(const std::string& text);
public:

	Converter() = default;

	Converter(const std::string& path);


/// ������
private:
	sqlite3* m_connection; ///< ����������
	std::string m_dbPath;  ///< ���� � ���� ������, ����� �������� ����������� ���� ����������
	std::string name_db;
	std::vector<std::vector<Var>> table;
	

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
	template<typename Val>
	inline Val& GetValue(Var& elem);

};

std::string quotesql(const std::string& s);

std::vector<Var> CheckSplit(const std::string& text);

template<typename Val>
inline Val& Converter::GetValue(Var& elem)
{
	return std::visit([](const auto& arg)
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, int>)
				return arg;
			else if constexpr (std::is_same_v<T, long>)
				return arg;
			else if constexpr (std::is_same_v<T, double>)
				return arg;
			else if constexpr (std::is_same_v<T, std::string>)
				return arg;
			/*else
				static_assert(false, "non-exhaustive visitor!");*/
		}, elem);

	/*using T = std::decay_t<decltype(elem)>;
	if constexpr (std::is_same_v<T, int>)
		return std::get<int>(elem);
	else if constexpr (std::is_same_v<T, std::nullptr_t>)
		return std::nullptr_t;
	else if constexpr (std::is_same_v<T, double>)
		return std::get<double>(elem);
	else if constexpr (std::is_same_v<T, std::string>)
		return std::get<double>(elem);
	else
		static_assert(false, "non-exhaustive visitor!");*/
}
