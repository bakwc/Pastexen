#include "uexception.h"
#include "ucolonsep.h"
#include <iostream>

#include <sstream>

#include <fstream>
#include <cstdio>
#include <cstdlib>

UColonSep::UColonSep()
{
}


std::vector<std::string> UColonSep::split(const std::string &s, char delit) { //custom split
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delit)) {
        elems.push_back(item);
    }
    return elems;
}

void UColonSep::Load(const std::string &fileName) {
    std::ifstream file(fileName);

    if (!file) {
        throw UException("Error opening file " + fileName);
    }
    std::string line = "";

    while (std::getline(file, line, '\n')) {
        try{
            if (line.empty() || line[0] == '#') {
                continue;
            }
            std::vector<std::string> objParams = split(line, ':');
            std::vector<UFromStringFormat> objs;

            for (int i = 0; i < objParams.size(); i++) {
                objs.push_back(objParams[i]);
            }
            Objects.push_back(objs);
            } catch (...) {
                file.close();
                throw;
        }

    }
}

size_t UColonSep::Rows() {
    return Objects.size();
}

size_t UColonSep::Cols(size_t rowNum) {
    if (rowNum > (size_t)Objects.size()) {
        throw UException("Index out of bounds");
    }
    return Objects[rowNum].size();
}

UFromStringFormat UColonSep::Get(size_t row, size_t col) {
    if (row > (size_t)Objects.size()
            || col > (size_t)Objects[row].size())
    {
        throw UException("Index out of bounds");
    }
    return Objects[row][col];
}
