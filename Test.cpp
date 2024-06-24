#include "stdafx.h"
#include <afxdllx.h>
#include "Test.h"
#include "DataBase.h"

int GetAllGOSTCallback(void* result, int count, char** rowData, char**ColNames) {
    SpecBladingProfileParams* resultdata = reinterpret_cast<SpecBladingProfileParams*>(result);
    if (resultdata) {
        VariableMap vars;
        for (int i = 2; i < count; i++) {
            std::wstring col_name = GetWStringFromString((std::string)ColNames[i]);
            vars.insert(std::make_pair((LPTSTR)(LPCTSTR)col_name.c_str(), std::stod(rowData[i])));
        }
        std::wstring num = GetWStringFromString((std::string)rowData[1]);
        resultdata->push_back(std::make_pair((LPTSTR)(LPCTSTR)num.c_str(), vars));
    }
    return 0;
}



bool SpecBladingDataLoader::ReadGOST8509Params(SpecBladingProfileParams& params, const std::wstring& InFile) {
    bool res = false;

    if (auto db = DataBase::GetDBConnection(InFile)) {
        auto file = std::filesystem::path(InFile.c_str()).filename();
        std::string name_file = file.string();
        auto it = name_file.find_last_of('.');
        auto name_db = (std::string)(name_file.substr(0, it));
        SpecBladingProfileParams tmp_params(params);
        std::string query = "SELECT * FROM ";
        query.append(quotesql(name_db));
        res = db->ExecuteQuery(query, GetAllGOSTCallback, &tmp_params);
        params = tmp_params;
    }
    return res;
}