#include <contrib/cimg/cimg.h>
#include <contrib/lodepng/lodepng.h>

#include "thumb_generator.h"

typedef cimg_library::CImg<unsigned char> TImage;

TImage LoadImageFile(const string& sourceFile) {
    // todo: load image using lodepng and other libs
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
