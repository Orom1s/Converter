#pragma once

#include "sqlite/sqlite3.h"


#pragma warning (disable : 26495)


class Converter {
public:

	Converter() = default;

	Converter(const std::string& path);


/// Данные
private:
	sqlite3* m_connection; ///< Соединение
	std::string m_dbPath;  ///< Путь к базе данных, после создания обновленной базы заменяется
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

	enum Coding {
		tc_unknown,
		tc_ansi,
		tc_unicode,
	};


	std::vector<ColumnSQL> table_;

private:

	/// Создать базу данных с именем loa файла
	bool CreateDBfile();
	bool Create();
	/// Окрыть соединение
	bool Open();
	/// Закрыть соединение
	bool Close();

	bool FillDBFile();

/// Работа с текстом
private:
	void Parse(const std::string& path);
	void ParseFromUtf8(const std::string& path);
	void ParseFromUtf16(const std::string& path);
	/// Разбить полученную строку на вектор со словами
	std::vector<std::string> SplitIntoWords(const std::string& text);

	std::vector<std::wstring> SplitIntoWords(const std::wstring& text);

	Type FoundType(std::string_view str);

	std::vector<std::string> FoundTypeForCol(const std::vector<std::string>& row);

	bool FoundDuplicate(std::vector<std::string>& cols);

	Coding CheckCoding();
};

bool ForEachFilesInDir(std::string path_to_directory);

