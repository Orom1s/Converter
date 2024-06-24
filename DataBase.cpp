#include "stdafx.h"
#include "DataBase.h"




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
            std::cerr << "DB file not found\n";
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



//------------------------------------------------------------------------------
/**
  возвращает полное имя файла
*/
//---
bool GetFullName(const std::wstring& inName, std::wstring& outName)
{
    bool res = false;

   auto fullName = std::filesystem::path(outName).parent_path();
   res = std::filesystem::exists(fullName.append(inName));

   if (!res)
       fullName.append(inName);

   if (res = std::filesystem::exists(fullName))
       outName = fullName.wstring();
    

    return res;
}


std::string quotesql(const std::string& s) {
    return std::string("\"") + s + std::string("\"");
}

//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::string в std::wstring
*/
//---
std::wstring GetWStringFromString(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::wstring в std::string
*/
//---
std::string GetStringFromWString(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr.c_str());
}