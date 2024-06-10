#include "Converter.h"

using namespace std::string_literals;

Converter::Converter(const std::string& path) : m_dbPath(path), table(10) {
	if (std::filesystem::exists(std::filesystem::path{ m_dbPath })) {
		Parse(std::ifstream{ m_dbPath });
	}	if (!table.empty()) {
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
			"CREATE TABLE "s + quotesql(name_db) + "(id INTEGER PRIMARY KEY);"s
			"ALTER TABLE "s + quotesql(name_db) + "ADD d INT;"s
			"INSERT INTO " + quotesql(name_db) +  "(d) VALUES (1, 2);"s;
		//for (auto vec : table) {
		//	Var name_col;
		//	bool first;
		//	for (auto elem : vec) {
		//		//std::int32_t status = sqlite3_exec(m_connection, sqlQuery.c_str(), callback, result, &messageError);
		//	}

		//}
		int rc = sqlite3_exec(m_connection, query.c_str(), 0, 0, &messageError);
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
	std::vector<Var> words;
	std::string line;
	int current_row = 0;
	bool flag_name_column = false;
	int count_comms = 0;
	while (getline(file, line)) {
		if (line[0] == 47 || line[0] == '*') {
			++count_comms;
			continue;
			
		}
		if (count_comms < 2) continue;
		if (count_comms == 3) {
			words = SplitIntoRows(line);
			for ()
			continue;
		}
		words = SplitIntoRows(line);
		int  i = 0;
		for (auto val : words) {

			table[i].push_back(val);
			++i;
		}
		++current_row;
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
	std::vector<Var> words;
	std::string word;
	Type type = NULL_;
	for (const char c : text) {
		if (c == '"') {
			if (type == NULL_) {
				type = TEXT;
			}
			else type = NULL_;
		}
		if ((c == ' ' || c == '\t') && type != TEXT) {
			if (!word.empty()) {
				type = FoundType(word);
				
				word.clear();
			}
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
	std::vector<std::string> type_for_col(row.size());
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



std::string quotesql(const std::string& s) {
	return std::string("'") + s + std::string("'");
}


std::vector<Var> CheckSplit(const std::string& text) {
	Converter converter;
	return converter.SplitIntoWords(text);
}

