#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "uexception.h"

template<typename T> T FromString(const std::string& str);

class UFromStringFormat {
public:
    UFromStringFormat(const std::string& str = std::string()) : Str(str)
    {
    }
    template <typename T> operator T() const {
        return FromString<T>(Str);
    }
private:
    std::string Str;
};

inline UFromStringFormat FromString(const std::string& str) {
    return UFromStringFormat(str);
}

template <typename T>
inline std::string ToString(const T& t) {
    return std::to_string(t);
}

inline std::string ToString(const std::vector<char> byteArray) {
    return std::string (byteArray.begin(), byteArray.end());
}
