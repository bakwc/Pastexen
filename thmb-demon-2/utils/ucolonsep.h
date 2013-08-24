#pragma once

#include <vector>
#include "ucast.h"
#include <iostream>
#include <string>
#include "uexception.h"

class UColonSep {
public:
    UColonSep();
    void Load(const std::string& fileName);
    size_t Rows();
    size_t Cols(size_t rowNum);
    UFromStringFormat Get(size_t row, size_t col);
    //std::vector<std::string> &split(const std::string &s, char delit, std::vector<std::string> &elems); //&split вызывается из split
    std::vector<std::string> split(const std::string &s, char delit);
private:
    std::vector<std::vector<UFromStringFormat> > Objects;
};
