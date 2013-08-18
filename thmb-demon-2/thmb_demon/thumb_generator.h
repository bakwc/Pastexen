#pragma once

#include <string>

#include "defines.h"

using namespace std;

void GenerateThumbnail(EFileType type,
                       const string& sourceFile,
                       const string& destFile,
                       size_t width,
                       size_t height);
