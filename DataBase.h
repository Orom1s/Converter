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

/// �������������� ������ �� std::string � std::wstring
std::wstring GetWStringFromString(const std::string& str);
/// �������������� ������ �� std::wstring � std::string
std::string GetStringFromWString(const std::wstring& wstr);

/// ���������� ������ ��� �����
bool GetFullName(const std::wstring& inName, std::wstring& outName);

/// ��������� ������ �� �������
std::wstring LoadStr(int id);
/// ������ ����������� ��������� �� ��������
int LibMessage(int strId, int flags);
/// ������ ��������� �����������
int LibMessage(const std::wstring& str, int flags);