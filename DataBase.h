#pragma once

#include "sqlite/sqlite3.h"

struct sqlite3;

/// Функция Callback для получения данных с базы
using CallBackFunction = int (*)(void*, int, char**, char**);

////////////////////////////////////////////////////////////////////////////////
//
/// Доступ к базе данных
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class DataBase
{
private:
	sqlite3* m_connection; ///< Соединение
	std::string m_dbPath;   ///< Путь к базе данных

public:
	/// Получить соединение с базой данных
	static DataBase* GetDBConnection(const std::wstring& path);

public:
	/// Конструктор
	DataBase(const std::string& dbPath);
	/// Выполнить запрос к базе
	bool ExecuteQuery(std::string sqlQuery, CallBackFunction callback, void* result);

private:
	/// Окрыть соединение
	bool Open();
	/// Закрыть соединение
	bool Close();
};


/// Возвращает полное имя файла
bool GetFullName(const std::wstring& inName, std::wstring& outName);
