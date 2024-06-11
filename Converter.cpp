﻿#include "Converter.h"

using namespace std::string_literals;

Converter::Converter(const std::string& path) : m_dbPath(path), table_(10) {
	if (std::filesystem::exists(std::filesystem::path{ m_dbPath })) {
		Parse(std::ifstream{ m_dbPath });
	}	
	if (!table_.empty()) {
		CreateDBfile();
		FillDBFile();
	}
}


bool Converter::CreateDBfile() {
	auto file = std::filesystem::path(m_dbPath).filename();
	std::string name_file = file.string();
	auto it = name_file.find_last_of('.');
	name_db = (std::string)(name_file.substr(0, it));
	auto directory = std::filesystem::path(m_dbPath).parent_path();
	std::filesystem::current_path(directory);
	std::filesystem::directory_entry entry{ std::filesystem::current_path() };
	std::filesystem::path dbfile{ name_db + ".db"};
	std::ofstream{ dbfile };
	entry.assign(entry / dbfile);
	if (entry.exists()) {
		/*if (!Create()) {
			std::cerr << "fail_create \n";
			Close();
			
		}*/
		m_dbPath = entry.path().string();
		return true;
	}
	return false;
}

bool Converter::Create() {
	return sqlite3_open_v2(m_dbPath.c_str(), &m_connection, SQLITE_OPEN_CREATE, NULL) == SQLITE_OK;
}

bool Converter::Open()
{
	return sqlite3_open_v2(m_dbPath.c_str(), &m_connection, SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK;
}

bool Converter::Close()
{
	return sqlite3_close(m_connection) == SQLITE_OK;
}

bool Converter::FillDBFile()
{
	char* messageError;
	if(Open()) {
		std::string query = "DROP TABLE IF EXISTS "s + quotesql(name_db) + ";"s
			"CREATE TABLE "s + quotesql(name_db) + "(id INT"s;
		for (int i = 0; i < table_.size(); ++i) {
			query += ", "s + quotesql(table_[i].name) + " "s + table_[i].type;
		}
		query += ");"s;
		int rc = sqlite3_exec(m_connection, query.c_str(), 0, 0, &messageError);
		if (rc != SQLITE_OK) {
			std::cerr << messageError << "\n";
			sqlite3_free(messageError);
			Close();
			return false;
		}
		for (int i = 0; i < table_[0].rows.size(); ++i) {
			query += "INSERT INTO"s + quotesql(name_db) + "VALUES(" + std::to_string(i+1);
			for (int j = 0; j < table_.size(); ++j) {
				query += ", " + quotesql(table_[j].rows[i]);
			}
			query += ");"s;
			rc = sqlite3_exec(m_connection, query.c_str(), 0, 0, &messageError);
			if (rc != SQLITE_OK) {
				std::cerr << messageError << "\n";
				sqlite3_free(messageError);
				Close();
				return false;
			}
		}

		rc = sqlite3_exec(m_connection, query.c_str(), 0, 0, &messageError);
		if (rc != SQLITE_OK) {
			std::cerr << messageError << "\n";
			sqlite3_free(messageError);
			Close();
			return false;
		}


	}
	else {
		std::cerr << "fail_open";
		Close();
		return false;
	}
	return true;
}

void Converter::Parse(std::ifstream file)
{
	std::vector<std::string> words;
	std::string line;
	bool flag_type_column = true;
	int count_comms = 0;
	while (getline(file, line)) {
		if (line[0] == 47 || line[0] == '*') {
			++count_comms;
			continue;
			
		}
		if (count_comms < 2) continue;
		if (count_comms == 2) {
			words = SplitIntoWords(line);
			table_.resize(words.size());
			FoundDuplicate(words);
			for (int i = 0; i < words.size(); ++i) {
				table_[i].name = words[i];
			}
			continue;
		}
		words = SplitIntoWords(line);
		if (count_comms == 3 && flag_type_column) {
			auto types = FoundTypeForCol(words);
			for (int i = 0; i < types.size(); ++i) {
				table_[i].type = types[i];
			}
			flag_type_column = false;
		}
		int  i = 0;
		for (auto val : words) {

			table_[i].rows.push_back(val);
			++i;
		}
	}
}




Converter* Converter::GetDBConnection() {
	return nullptr;
}

std::string Converter::GetDBPath()
{
	return m_dbPath;
}

std::vector<std::string> Converter::SplitIntoWords(const std::string& text) {
	std::vector<std::string> words;
	std::string word;
	for (const char c : text) {
		if (c == ' ' || c == '\t') {
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}
			continue;
		}
		else {
			word += c;
		}
	}
	if (!word.empty()) {
		words.push_back(word);
	}
	return words;
}	


Converter::Type Converter::FoundType(std::string_view str) {
	auto it1 = std::find_if(str.begin(), str.end(), [](const char c) {
		return (c == '"');
		});
	if (it1 != str.end()) {
		return Type::TEXT;
	}
	auto it = std::find_if(str.begin(), str.end(), [](const char c) {
		return (std::isalpha(c));
		});
	
	if (it == str.end()) {
		auto it = std::find_if(str.begin(), str.end(), [](const char c) {
			return (std::isdigit(c));
			});
		auto it1 = std::find_if(str.begin(), str.end(), [](const char c) {
			return (c == '.');
			});
		if (it != str.end()) {
			if (it1 != str.end()) {
				return Type::DOUBLE;
			}
			return Type::INT;
		}
	}
	return Type::TEXT;
}

std::vector<std::string> Converter::FoundTypeForCol(const std::vector<std::string>& row) {
	std::vector<std::string> type_for_col;
	for (auto word : row) {
		Type type = FoundType(word);
		std::string str_type;
		switch (type)
		{
		case TEXT:
			str_type = "TEXT"s;
			break;
		case INT:
			str_type = "INT"s;
			break;
		case DOUBLE:
			str_type = "REAL"s;
			break;
		}
		type_for_col.push_back(str_type);
	}
	return  type_for_col;
}

bool Converter::FoundDuplicate(std::vector<std::string>& cols)
{
	std::vector<std::string> non_dup(cols.size());
	for (int i = 0; i < cols.size() - 1; ++i) {
		int count = 0;
		for (int j = i + 1; j < cols.size(); ++j) {
			if (iequals(cols[i], cols[j])) {
				++count;
				cols[j] += "(" + std::to_string(count) + ")";
			}
		}
	}
	return true;
}

bool ichar_equals(char a, char b)
{
	return std::tolower(static_cast<unsigned char>(a)) ==
		std::tolower(static_cast<unsigned char>(b));
}

bool iequals(std::string_view lhs, std::string_view rhs)
{
	return std::ranges::equal(lhs, rhs, ichar_equals);
}



std::string quotesql(const std::string& s) {
	return std::string("\""s) + s + std::string("\""s);
}

std::string ReadLine(std::istream& input) {
	std::string s;
	std::getline(input, s);
	return s;
}

bool ForEachFilesInDir(std::string path_to_directory) {
	std::filesystem::path input_path(path_to_directory);
	for (auto& dir_entry : std::filesystem::recursive_directory_iterator(input_path)) {
		if (dir_entry.is_regular_file()) {
			auto name_file = dir_entry.path().filename().string();
			auto it = name_file.find_last_of('.');
			std::string extension = (std::string)(name_file.substr(it));
			if (extension == ".loa") {
				Converter(dir_entry.path().string());
			}
		}
	}
	return true;
}