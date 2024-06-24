#pragma once

typedef std::map<CString, double> VariableMap;
typedef std::pair<CString, VariableMap> ParamPair;
typedef std::vector<ParamPair> SpecBladingProfileParams;


struct SpecBladingDataLoader {
#define LOA_SPECIAL_BLADING_GOST8509_PATH const_cast<wchar_t*>(_T("Sys\\GOST-8509-93.loa"))
	bool ReadGOST8509Params(SpecBladingProfileParams& params);
};