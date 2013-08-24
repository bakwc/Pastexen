#pragma once

#include <string>
#include <map>
#include <iostream>
#include "ucast.h"

class USettings{
public:
	typedef std::map<std::string, std::string> TParametersHash;
public:
    USettings();
    virtual ~USettings() {}
	void Load(const std::string& fname, const std::string& required = std::string());
	void Save(const std::string& fname, bool keepOrigin = false);
	UFromStringFormat GetParameter(const std::string& parameter);
	void SetParameter(const std::string& parameter, const std::string& value);
    void DefineParams(const TParametersHash& paramsList);
private:
    TParametersHash Parameters;
};
