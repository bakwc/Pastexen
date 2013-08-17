#include <contrib/lodepng/lodepng.h>
#include <contrib/cimg/cimg.h>
#include <boost/algorithm/string.hpp>
#include "thumb_generator.h"

using namespace boost::algorithm;

typedef cimg_library::CImg<unsigned char> TImage;

TImage LoadPng(const string& sourceFile) {
    std::vector<unsigned char> image;
    size_t width, height;
    size_t err = lodepng::decode(image, width, height, sourceFile);
    if (err) {
        std::cerr << lodepng_error_text(err);
        return TImage();
    }
    TImage result(width, height, 1, 3);
    unsigned char* r = result.data(0, 0, 0, 0);
    unsigned char* g = result.data(0, 0, 0, 1);
    unsigned char* b = result.data(0, 0, 0, 2);
    const unsigned char* s;
    for (s = image.data(); s < image.data() + image.size();) {
        *(r++) = *(s++);
        *(g++) = *(s++);
        *(b++) = *(s++);
        s++;
    }
    return result;
}

TImage LoadImageFile(const string& sourceFile) {
    if (ends_with(sourceFile, ".bmp")) {
        return TImage(sourceFile.c_str());
    } else if (ends_with(sourceFile, ".png")) {
        return LoadPng(sourceFile);
    }
    // todo: implement loading jpg
    return TImage();
}

void SaveImage(const TImage& image, const string& destFile) {
    // todo: save image using lodepng
}

TImage MakeImageThumb(const string& sourceFile, size_t width, size_t height) {
    TImage baseImage = LoadImageFile(sourceFile);
    // todo: resize baseImage
    return baseImage;
}

TImage MakeTextThumb(const string& sourceFile, size_t width, size_t height) {
    string text;
    // todo: load text from sourceFile
    TImage thumb(100, 100, 1, 3, 0);
    // todo: write text to thumb
    // todo: resize thumb
    return thumb;
}

void GenerateThumbnail(EFileType type,
                       const string& sourceFile,
                       const string& destFile,
                       size_t width,
                       size_t height)
{
    TImage thumb;
    if (type == FT_Image) {
        thumb = MakeImageThumb(sourceFile, width, height);
    } else if (type == FT_Text) {
        thumb = MakeTextThumb(sourceFile, width, height);
    } else {
        // todo: throw exception
    }
    SaveImage(thumb, destFile);
}
