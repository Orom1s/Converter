#pragma once


struct sqlite3;

/// ������� Callback ��� ��������� ������ � ����
using CallBackFunction = int (*)(void*, int, char**, char**);

////////////////////////////////////////////////////////////////////////////////
//
/// ������ � ���� ������
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class DataBase
{
private:
	sqlite3* m_connection; ///< ����������
	std::string m_dbPath;   ///< ���� � ���� ������

public:
	/// �������� ���������� � ����� ������
	static DataBase* GetDBConnection(const std::wstring& path);

public:
	/// �����������
	DataBase(const std::string& dbPath);
	/// ��������� ������ � ����
	bool ExecuteQuery(std::string sqlQuery, CallBackFunction callback, void* result);

private:
	/// ������ ����������
	bool Open();
	/// ������� ����������
	bool Close();
};


/// ���������� ������ ��� �����
bool GetFullName(const std::wstring& inName, std::wstring& outName);

std::string quotesql(const std::string& s);

std::wstring GetWStringFromString(const std::string& str);

std::string GetStringFromWString(const std::wstring& wstr);
