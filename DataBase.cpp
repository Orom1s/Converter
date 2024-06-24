#include "stdafx.h"
#include "DataBase.h"


#include <filesystem>
#include <memory>

#include "sqlite/sqlite3.h"


//------------------------------------------------------------------------------
/**
  Получить соединение с базой данных
*/
//---
DataBase* DataBase::GetDBConnection(const std::wstring& path)
{
    static std::unique_ptr<DataBase> db;

    if (!db)
    {
        std::wstring outPath;

        if (GetFullName(path.c_str(), outPath))
        {
            std::wstring dbPath(outPath);
            db = std::make_unique<DataBase>(GetStringFromWString(dbPath));
        }
        else
            LibMessage(LoadStr(ID_DB_NOT_FOUND_ERROR), MB_ICONWARNING);
    }

    return db.get();
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
DataBase::DataBase(const std::string& dbPath)
    : m_dbPath(dbPath)
{
}


//------------------------------------------------------------------------------
/**
  Окрыть соединение
*/
//---
bool DataBase::Open()
{
    return sqlite3_open_v2(m_dbPath.c_str(), &m_connection, SQLITE_OPEN_READONLY, NULL) == SQLITE_OK;
}


//------------------------------------------------------------------------------
/**
  Выполнить запрос к базе
*/
//---
bool DataBase::ExecuteQuery(std::string sqlQuery, CallBackFunction callback, void* result)
{
    if (Open() && m_connection)
    {
        char* messageError;
        std::int32_t status = sqlite3_exec(m_connection, sqlQuery.c_str(), callback, result, &messageError);

        if (status != SQLITE_OK)
        {
            LibMessage(LoadStr(ID_DB_ERROR).append(GetWStringFromString(messageError)), MB_ICONWARNING);
            sqlite3_free(messageError);
            return false;
        }

        Close();
        return true;
    }

    return false;
}


//------------------------------------------------------------------------------
/**
  Закрыть соединение
*/
//---
bool DataBase::Close()
{
    return sqlite3_close(m_connection) == SQLITE_OK;
}


