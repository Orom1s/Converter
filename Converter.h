#pragma once

#include "sqlite/sqlite3.h"


#pragma warning (disable : 26495)


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

	enum Coding {
		tc_unknown,
		tc_ansi,
		tc_unicode,
	};


	std::vector<ColumnSQL> table_;

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
	void Parse(const std::string& path);
	void ParseFromUtf8(const std::string& path);
	void ParseFromUtf16(const std::string& path);
	/// ������� ���������� ������ �� ������ �� �������
	std::vector<std::string> SplitIntoWords(const std::string& text);

	std::vector<std::wstring> SplitIntoWords(const std::wstring& text);

	Type FoundType(std::string_view str);

	std::vector<std::string> FoundTypeForCol(const std::vector<std::string>& row);

	bool FoundDuplicate(std::vector<std::string>& cols);

	Coding CheckCoding();
};

bool ForEachFilesInDir(std::string path_to_directory);

