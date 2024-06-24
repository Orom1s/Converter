#include "stdafx.h"
#include "DataBase.h"
#include "Resource.h"

#include <filesystem>
#include <memory>
#include<codecvt>

#include "sqlite/sqlite3.h"

extern ksAPI7::IApplication* application;
extern void* GetModule();

//------------------------------------------------------------------------------
/**
  �������� ���������� � ����� ������
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
  �����������
*/
//---
DataBase::DataBase(const std::string& dbPath)
    : m_dbPath(dbPath)
{
}


//------------------------------------------------------------------------------
/**
  ������ ����������
*/
//---
bool DataBase::Open()
{
    return sqlite3_open_v2(m_dbPath.c_str(), &m_connection, SQLITE_OPEN_READONLY, NULL) == SQLITE_OK;
}


//------------------------------------------------------------------------------
/**
  ��������� ������ � ����
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
  ������� ����������
*/
//---
bool DataBase::Close()
{
    return sqlite3_close(m_connection) == SQLITE_OK;
}


//------------------------------------------------------------------------------
/**
  �������������� ������ �� std::string � std::wstring
*/
//---
std::wstring GetWStringFromString(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}


//------------------------------------------------------------------------------
/**
  �������������� ������ �� std::wstring � std::string
*/
//---
std::string GetStringFromWString(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr.c_str());
}

//------------------------------------------------------------------------------
/**
  ���������� ������ ��� �����
*/
//---
bool GetFullName(const std::wstring& inName, std::wstring& outName)
{
    bool res = false;
    outName = GetModuleFilePath(GetModule());

    if (!outName.empty())
    {
        auto fullName = std::filesystem::path(outName).parent_path();
        res = std::filesystem::exists(fullName.append(inName));

        if (!res && application)
        {
            std::wstring outPath;

            if (ksAPI7::ISystemSettingsPtr settings = application->GetSystemSettings())
            {
                fullName = settings->GetSystemPath(ksSystemFiles);
                fullName.append(inName);
            }
        }

        if (res = std::filesystem::exists(fullName))
            outName = fullName.wstring();
    }

    return res;
}



//------------------------------------------------------------------------------
/**
  ��������� ������ �� �������
*/
//---
std::wstring LoadStr(int id)
{
    std::wstring resStr;

    if (application)
    {
        if (ksapi::ILibraryManagerPtr libMng = application->GetLibraryManager())
        {
            if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
            {
                if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItem(L"��������������� ������� �� Visual C++ � �������"))
                {
                    ksapi::IKompasDictionary& dictionary = procLib->GetDictionary();
                    resStr = dictionary.LoadAndConvertLangString(GetModule(), id);
                }
            }
        }
    }

    return resStr;
}


//------------------------------------------------------------------------------
/**
  ����� ���������
*/
//---
int LibMessage(const std::wstring& str, int flags)
{
    if (!str.empty() && application)
        // ����� ���������, ���������, ���������
        return application->MessageBoxEx(str, LoadStr(ID_LIBID), flags);

    return 0;
}


//------------------------------------------------------------------------------
/**
  ������ ����������� ��������� �� ��������
*/
//---
int LibMessage(int strId, int flags)
{
    return ::LibMessage(LoadStr(strId), flags);
}