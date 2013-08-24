#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <contrib/lodepng/lodepng.h>
#define cimg_display 0
#include <contrib/cimg/cimg.h>
#include <contrib/jpeg/jpgd.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <contrib/stb_truetype/stb_truetype.h>

#include <utils/uexception.h>
#include <utils/string.h>

#include "thumb_generator.h"

using namespace boost::algorithm;

typedef cimg_library::CImg<unsigned char> TImage;

TImage LoadPng(const string& sourceFile) {
    vector<unsigned char> image;
    unsigned int width, height;
    size_t err = lodepng::decode(image, width, height, sourceFile, LCT_RGBA, 8);
    if (err) {
        throw UException(lodepng_error_text(err));
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
        throw UException("Failed to load jpeg file " + sourceFile);
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
    throw UException("Failed to load image file - unknown type");
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
        throw UException("failed to save image - wrong type");
    }
    size_t err = lodepng::encode(destFile, result, image.width(), image.height());
    if (err) {
        throw UException(lodepng_error_text(err));
    }
}

class TThumbGeneratorImpl {
public:
    TThumbGeneratorImpl(const string& fontFile, size_t width, size_t height);
    void GenerateThumbnail(EFileType type, const string& sourceFile, const string& destFile);
private:
    void DrawText(TImage& image, size_t x, size_t y, const wstring& wtext, size_t size);
    TImage MakeTextThumb(const string& sourceFile);
    TImage MakeImageThumb(const string& sourceFile);
private:
    string FontData;
    stbtt_fontinfo Font;
    size_t Width;
    size_t Height;
};

TThumbGeneratorImpl::TThumbGeneratorImpl(const string& fontFile, size_t width, size_t height)
    : FontData(LoadFile(fontFile))
    , Width(width)
    , Height(height)
{
    stbtt_InitFont(&Font, (const unsigned char*)FontData.c_str(), 0);
}

void TThumbGeneratorImpl::GenerateThumbnail(EFileType type,
                                                const string &sourceFile,
                                                const string &destFile)
{
    TImage thumb;
    if (type == FT_Image) {
        thumb = MakeImageThumb(sourceFile);
    } else if (type == FT_Text) {
        thumb = MakeTextThumb(sourceFile);
    } else {
        throw UException("GenerateThumb: unknown type");
    }
    SaveImage(thumb, destFile);
}

TImage TThumbGeneratorImpl::MakeImageThumb(const string& sourceFile) {
    // todo: correct thumbs for images width / height rate not only 1:1
    TImage image = LoadImageFile(sourceFile);
    if (image.width() > image.height()) {
        image = image.get_crop(0, 0, image.height(), image.height(), true);
    } else if (image.height() > image.width()) {
        image = image.get_crop(0, 0, image.width(), image.width(), true);
    }
    return image.get_resize(Width, Height, -100, -100, 5);
}

void TThumbGeneratorImpl::DrawText(TImage& image, size_t x, size_t y, const wstring& wtext, size_t size) {
    float scale = stbtt_ScaleForPixelHeight(&Font, size);

    int w, h, xo, yo;
    unsigned char* r = image.data(0, 0, 0, 0);
    unsigned char* g = image.data(0, 0, 0, 1);
    unsigned char* b = image.data(0, 0, 0, 2);

    size_t pos = x;
    for (size_t i = 0; i < wtext.size(); i++) {
        if (iswspace(wtext[i])) {
            pos += size * 0.3;
            continue;
        }
        unsigned char* d = stbtt_GetCodepointBitmap(&Font, 0, scale, wtext[i], &w, &h, &xo, &yo);
        for (size_t wx = 0; wx < w; wx++) {
            for (size_t wy = 0; wy < h; wy++) {
                size_t cx = pos + wx + xo;
                size_t cy = y + wy + yo + size;
                if (cy >= image.height() ||
                        cx >= image.width())
                {
                    continue;
                }
                size_t dcord = image.width() * cy + cx;
                size_t scord = wy * w + wx;
                r[dcord] = d[scord];
                g[dcord] = d[scord];
                b[dcord] = d[scord];
            }
        }
        pos += w;
    }
}

TImage TThumbGeneratorImpl::MakeTextThumb(const string& sourceFile) {
    string text = LoadFile(sourceFile);
    // Removing UTF8 BOM (239 187 191)
    if (text.length() >= 3 && (uint8_t)text[0] == 239 && (uint8_t)text[1] == 187 && (uint8_t)text[2] == 191) {
        text.erase(text.begin(), text.begin() + 3);
    }
    wstring wtext = UTF8ToWide(text);
    vector<wstring> lines;
    split(lines, wtext, is_any_of("\n\r"));
    size_t n = 0;
    TImage thumb(Width, Height, 1, 3, 0);

    for (size_t i = 0; i < lines.size(); ++i) {
        wstring& text = lines[i];
        trim(text);
        if (!text.empty()) {
            DrawText(thumb, 6, 9 * n, text, 10);
            ++n;
            if (9 * n > Width + 9) break;
        }
    }
    return thumb;
}

TThumbGenerator::TThumbGenerator(const string& fontFile, size_t width, size_t height)
    : Impl(new TThumbGeneratorImpl(fontFile, width, height))
{
}

TThumbGenerator::~TThumbGenerator() {
}

void TThumbGenerator::GenerateThumbnail(EFileType type,
                                        const string& sourceFile,
                                        const string& destFile)
{
    Impl->GenerateThumbnail(type, sourceFile, destFile);
}
