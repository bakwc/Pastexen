#include <iostream>
#include <boost/algorithm/string.hpp>
#include <contrib/lodepng/lodepng.h>
#include <contrib/cimg/cimg.h>
#include <contrib/jpeg/jpgd.h>

#include "thumb_generator.h"

using namespace boost::algorithm;

typedef cimg_library::CImg<unsigned char> TImage;

TImage LoadPng(const string& sourceFile) {
    vector<unsigned char> image;
    size_t width, height;
    size_t err = lodepng::decode(image, width, height, sourceFile);
    if (err) {
        cerr << lodepng_error_text(err);
        return TImage();
    }
    TImage result(width, height, 1, 4);
    unsigned char* r = result.data(0, 0, 0, 0);
    unsigned char* g = result.data(0, 0, 0, 1);
    unsigned char* b = result.data(0, 0, 0, 2);
    unsigned char* a = result.data(0, 0, 0, 3);
    const unsigned char* s;
    for (s = image.data(); s < image.data() + image.size();) {
        *(r++) = *(s++);
        *(g++) = *(s++);
        *(b++) = *(s++);
        *(a++) = *(s++);
    }
    return result;
}

TImage LoadJpeg(const string& sourceFile) {
    int width, height, comps;
    unsigned char* image;
    image = jpgd::decompress_jpeg_image_from_file(sourceFile.c_str(), &width, &height, &comps, 4);
    if (!image) {
        cerr << "Failed to load jpeg image";
        return TImage();
    }
    TImage result(width, height, 1, 4);
    unsigned char* r = result.data(0, 0, 0, 0);
    unsigned char* g = result.data(0, 0, 0, 1);
    unsigned char* b = result.data(0, 0, 0, 2);
    unsigned char* a = result.data(0, 0, 0, 3);
    const unsigned char* s;
    for (s = image; s < image + width * height * 4;) {
        *(r++) = *(s++);
        *(g++) = *(s++);
        *(b++) = *(s++);
        *(a++) = *(s++);
    }
    return result;
}

TImage LoadImageFile(const string& sourceFile) {
    if (ends_with(sourceFile, ".bmp")) {
        return TImage(sourceFile.c_str());
    } else if (ends_with(sourceFile, ".png")) {
        return LoadPng(sourceFile);
    } else if (ends_with(sourceFile, ".jpg")) {
        return LoadJpeg(sourceFile);
    }
    // todo: implement loading gif and mb tiff
    return TImage();
}

// store image inside png
void SaveImage(const TImage& image, const string& destFile) {
    vector<unsigned char> result(image.width() * image.height() * 4);
    unsigned char* d;
    const unsigned char* r = image.data(0, 0, 0, 0);
    const unsigned char* g = image.data(0, 0, 0, 1);
    const unsigned char* b = image.data(0, 0, 0, 2);
    int sp = image.spectrum();
    if (sp == 4) {
        const unsigned char* a = image.data(0, 0, 0, 3);
        for (d = result.data(); d < result.data() + result.size();) {
            *(d++) = *(r++);
            *(d++) = *(g++);
            *(d++) = *(b++);
            *(d++) = *(a++);
        }
    } else if (sp == 3) {
        for (d = result.data(); d < result.data() + result.size();) {
            *(d++) = *(r++);
            *(d++) = *(g++);
            *(d++) = *(b++);
            *(d++) = 255;
        }
    } else {
        // todo: exception here
        cerr << "wrong image type";
        return;
    }
    size_t err = lodepng::encode(destFile, result, image.width(), image.height());
    if (err) {
        cerr << lodepng_error_text(err) << "\n";
    }
}

TImage MakeImageThumb(const string& sourceFile, size_t width, size_t height) {
    // todo: correct thumbs for images width / height rate not only 1:1
    TImage image = LoadImageFile(sourceFile);
    if (image.width() > image.height()) {
        image = image.get_crop(0, 0, image.height(), image.height(), true);
    } else if (image.height() > image.width()) {
        image = image.get_crop(0, 0, image.width(), image.width(), true);
    } else {
        // todo: throw exception
    }
    return image.get_resize(width, height, -100, -100, 5);
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
