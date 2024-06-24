#pragma once



typedef std::map<CString, double> VariableMap;
typedef std::pair<CString, VariableMap> ParamPair;
typedef std::vector<ParamPair> SpecBladingProfileParams;


struct SpecBladingDataLoader {
	bool ReadGOST8509Params(SpecBladingProfileParams& params, const std::wstring& InFile);
};