#pragma once

#include <string>
#include <memory>

#include "defines.h"

using namespace std;

class TThumbGeneratorImpl;
class TThumbGenerator {
public:
    TThumbGenerator(const string& fontFile, size_t width, size_t height);
    ~TThumbGenerator();
    void GenerateThumbnail(EFileType type,
                           const string &sourceFile,
                           const string &destFile);
private:
    unique_ptr<TThumbGeneratorImpl> Impl;
};
