#pragma once

#include "sqlite/sqlite3.h"


#pragma warning (disable : 26495)


class loatodb {
public:

	loatodb() = default;

	loatodb(const std::string& path);


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
	void Parse(const std::string& path, Coding type);
	/*template<class Input, class String>
	void Parse(Input& ifile, String& line);*/
	/// Разбить полученную строку на вектор со словами
	std::vector<std::string> SplitIntoWords(const std::string& text);

	std::vector<std::wstring> SplitIntoWords(const std::wstring& text);

	Type FoundType(std::string_view str);

	std::vector<std::string> FoundTypeForCol(const std::vector<std::string>& row);

	bool FoundDuplicate(std::vector<std::string>& cols);

	Coding CheckCoding();
};

bool ForEachFilesInDir(std::filesystem::path path_to_directory);

//template<class Input, class String>
//inline void loatodb::Parse(Input& file, String& str) {
//	bool is_wstring = false;
//	if (std::is_same<String, std::wstring>()) {
//		is_wstring = true;
//	}
//	std::vector<std::string> words;
//	bool flag_type_column = true;
//	ComBlock comms;
//	while (getline(file, str)) {
//		std::string line;
//		if (is_wstring) {
//			line = GetStringFromWString(str);
//		}
//		else {
//			line = str;
//		}
//		auto it = line.find('/');
//		auto it2 = line.find('*');
//		if (it != std::string::npos && it2 != std::string::npos) {
//			if (it < it2) {
//				comms.com_closed = false;
//				continue;
//			}
//			else {
//				comms.com_closed = true;
//				++comms.count;
//				continue;
//			}
//		}
//		if (!comms.com_closed) {
//			comms.buffer = line;
//			continue;
//		}
//		words = SplitIntoWords(line);
//		if (!words.empty()) {
//			if (flag_type_column) {
//				auto cols = SplitIntoWords(comms.buffer);
//				table_.resize(cols.size());
//				FoundDuplicate(cols);
//				for (int i = 0; i < cols.size(); ++i) {
//					table_[i].name = cols[i];
//				}
//				auto types = FoundTypeForCol(words);
//				for (int i = 0; i < types.size(); ++i) {
//					table_[i].type = types[i];
//				}
//				flag_type_column = false;
//			}
//			int  i = 0;
//			for (auto val : words) {
//
//				table_[i].rows.push_back(val);
//				++i;
//			}
//		}
//		else continue;
//	}
//}
