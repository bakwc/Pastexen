#include "ucast.h"
#include <stdexcept>
#include <string>
#include <sstream>

template <typename T>
T from_string(std::string const& s) {
    std::stringstream ss(s);
    T result;
    ss >> result;
    return result;
}

template< >
unsigned char FromString<unsigned char>(const std::string& str){
    unsigned char res=0;
    try{
        res = from_string<unsigned char>(str);
    }
    catch(std::invalid_argument&)
    {
        throw UException("Cast error");
    }
    return res;
}

template< >
int FromString<int>(const std::string& str){
    int res=0;
    try{
        res = from_string<int>(str);
    } catch(std::invalid_argument&) {
       throw UException("Cast error");
    }
    return res;
}

template< >
double FromString<double>(const std::string& str){
    double res=0;
    try{
        res = from_string<double>(str);
    }
    catch(std::invalid_argument&)
    {
        throw UException("Cast error");
    }
    return res;
}

template< >
float FromString<float>(const std::string& str){
    float res=0;
    try{
        res = from_string<float>(str);
    }
    catch(std::invalid_argument&)
    {
        throw UException("Cast error");
    }
    return res;
}

template< >
unsigned int FromString<unsigned int>(const std::string& str){//
    unsigned int res=0;
    try{
        res = from_string<unsigned int>(str);
    }
    catch(std::invalid_argument&)
    {
        throw UException("Cast error");
    }
    return res;
}

template< >
unsigned short FromString<unsigned short>(const std::string& str){
    unsigned short res=0;
    try{
        res = from_string<unsigned short>(str);
    }
    catch(std::invalid_argument&)
    {
        throw UException("Cast error");
    }
    return res;
}

template< >
unsigned long FromString<unsigned long>(const std::string& str){
    unsigned long res=0;
    try{
        res = from_string<unsigned long>(str);
    }
    catch(std::invalid_argument&)
    {
        throw UException("Cast error");
    }
    return res;
}

template< >
unsigned long long FromString<unsigned long long>(const std::string& str){
    unsigned long long res=0;
    try{
        res = from_string<unsigned long long>(str);
    }
    catch(std::invalid_argument&)
    {
        throw UException("Cast error");
    }
    return res;
}

template< >
bool FromString<bool>(const std::string& str){
    bool res = false;
    if (str == "1" || str == "true") {
       res = true;
    } else if (str == "0" || str == "false") {
       res = false;
    } else {
       throw UException("Cast error");
    }
    return res;
}

template< >
std::string FromString<std::string>(const std::string& str){
    return str;
}

template< >
std::vector<char> FromString<std::vector<char>>(const std::string& str){
    return std::vector<char>(str.begin(), str.end());
}
