#include "usettings.h"
#include <fstream>

#include <sstream>
#include <vector>
#include "uexception.h"

USettings::USettings()
{
}
void USettings::Load(const std::string& fname, const std::string& required) {
	std::ifstream file(fname);
    if (!file) {
        throw UException("Error opening file " + fname);
    }
	std::string line = "";
    while (!file.eof()) {
		file >> line;

		int i=0, j=line.size()-1;
		while (line[i]==' ')	i++;
		while( line[j]==' ')	j--;	
		line.erase(j+1,line.size());
		line.erase(0,i);

        if (line == "" || line[0] == '#') {
            continue;
        }
		std::vector<std::string> acc;
		std::stringstream ss(line);
		std::string s = "";
		while (std::getline(ss, s, '='))
			acc.push_back(s);
        if (acc.size() != 2) {
            continue;
        }
        Parameters.insert(std::pair<std::string, std::string>(acc[0], acc[1]));
    }
	
    if ( Parameters.find(required) == Parameters.end() ) {
		std::cout<<"Required parameter " + required + " not found"<<std::endl;
            //throw UException("Required parameter " + *i + " not found");
    }
}

UFromStringFormat USettings::GetParameter(const std::string& parameter) {
    if (Parameters.find(parameter) == Parameters.end()) {
        throw UException(std::string("Parameter " + parameter + "not found in config"));
    }
    return FromString(Parameters[parameter]);
}

void USettings::SetParameter(const std::string& parameter, const std::string& value)
{
    if( parameter == "")
        return;

	if(Parameters.find(parameter) != Parameters.end() )
		Parameters.erase(parameter);

    Parameters[parameter] = value;
}

void USettings::DefineParams(const TParametersHash &paramsList) {
    Parameters = paramsList;
}

void USettings::Save(const std::string &fname, bool keepOrigin)
{
    std::ofstream file(fname);

    if (!file) {
        throw UException("Error opening file for writing " + fname);
    }
	std::map<std::string, std::string>::iterator p;
	for(p = Parameters.begin(); p != Parameters.end(); p++){
		file << p->first << "=" << p->second << "\n";
    }
    file.flush();
}
