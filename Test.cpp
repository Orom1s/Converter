#include "stdafx.h"
#include <afxdllx.h>
#include "LHead3d.h"
#include "Test.h"
#include "DataBase.h"

int GetAllGOSTCallback(void* result, int count, char** rowData, char**ColNames) {
    SpecBladingProfileParams* resultdata = reinterpret_cast<SpecBladingProfileParams*>(result);
    if (resultdata) {
        VariableMap vars;
        for (int i = 2; i < count; i++) {
            //std::wstring col_name = GetWstringFromString((std::string)ColNames[i]);
            vars.insert(std::make_pair((LPTSTR)(LPCTSTR)ColNames[i], std::stod(rowData[i])));
        }
        resultdata->push_back(std::make_pair((LPTSTR)(LPCTSTR)ColNames[0], vars));
    }
}



#define LOA_SPECIAL_BLADING_GOST8509_PATH const_cast<wchar_t*>(_T("Sys\\GOST-8509-93.db"))
bool SpecBladingDataLoader::ReadGOST8509Params(SpecBladingProfileParams& params) {
    bool res = false;
#pragma message (TODO "// API22 MISSING")
    CString outName(KompasAppl::GetFullFileName(LOA_SPECIAL_BLADING_GOST8509_PATH, EasySteelhInstance));
    //DataBase db(ConvertLPCWSTRToString(outName.GetString()));

    if (auto db = DataBase::GetDBConnection(outName.GetString())) {
        SpecBladingProfileParams tmp_params(params);
        std::string query = "SELECT * FROM \"GOST-8509-93\"";
        bool res = db->ExecuteQuery(query, GetAllGOSTCallback, &tmp_params);
    }
    return res;
}