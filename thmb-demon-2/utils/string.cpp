#include <fstream>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <iconv.h>

#include "string.h"

string LoadFile(const string& fileName) {
    std::ifstream ifs;
    ifs.open(fileName, fstream::binary | fstream::in);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    return content;
}

void SaveFile(const string& fileName, const string& data) {
    std::ofstream ofs;
    ofs.open(fileName, fstream::binary | fstream::out);
    ofs.write(data.c_str(), data.size());
}

wstring UTF8ToWide(const string& text) {
    return boost::locale::conv::to_utf<wchar_t>(text, "UTF-8");
}

string WideToUTF8(const wstring& text) {
    return boost::locale::conv::from_utf<>(text, "UTF-8");
}

string RecodeText(string text, const string& from, const string& to) {
    if (text.empty()) {
        return text;
    }
    iconv_t cnv = iconv_open(to.c_str(), from.c_str());
    if (cnv == (iconv_t)-1) {
        iconv_close(cnv);
        return "";
    }

    char* outbuf;
    if ((outbuf = (char*)malloc(text.length() * 2 + 1)) == NULL) {
        iconv_close(cnv);
        return "";
    }

    char* ip = (char*)text.c_str();
    char* op = outbuf;
    size_t icount = text.length();
    size_t ocount = text.length() * 2;

    if (iconv(cnv, &ip, &icount, &op, &ocount) != (size_t)-1) {
        outbuf[text.length() * 2 - ocount] = '\0';
        text = outbuf;
    } else {
        text = "";
    }

    free(outbuf);
    iconv_close(cnv);
    return text;
}

bool isdatedelim(char c) {
    return c == '.' || c == ':' || c == '\\' || c == '/';
}

bool issentbreak(char c) {
    return c == '.' || c == '!' || c == '?';
}

bool iswordsymbol(wchar_t symbol) {
    return iswalpha(symbol) || iswdigit(symbol) || isdatedelim(symbol);
}

size_t CalcWordsCount(const string& text) {
    if (text.empty()) {
        return 0;
    }
    size_t count = 0;
    wstring wtext = UTF8ToWide(text + " ");
    for (size_t i = 1; i < wtext.size(); i++) {
        if (!iswordsymbol(wtext[i]) &&
                iswordsymbol(wtext[i - 1]) && (
                    i == 1 || iswordsymbol(wtext[i - 2])))
        {
            count++;
        }
    }
    return count;
}

size_t CalcDigitCount(const string& text) {
    size_t count = 0;
    for (size_t i = 0; i < text.size(); i++) {
        if (isdigit(text[i])) {
            count++;
        }
    }
    return count;
}

size_t CalcPunctCount(const string& text) {
    size_t count = 0;
    for (size_t i = 0; i < text.size(); i++) {
        if (ispunct(text[i])) {
            count++;
        }
    }
    return count;
}

bool HasPunct(const string& text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '.' ||
                text[i] == ',' ||
                text[i] == '?' ||
                text[i] == '!')
        {
            return true;
        }
    }
    return false;
}

string NormalizeText(const string& text, bool hard) {
    wstring wtext = UTF8ToWide(text);
    wstring current;
    wstring result;
    for (size_t i = 0; i < wtext.size(); i++) {
        if (iswspace(wtext[i]) ||
                wtext[i] == '<' ||
                wtext[i] == '>' ||
                (hard && wtext[i] == '.'))
        {
            wtext[i] = ' ';
        }
        if ((hard && iswalpha(wtext[i])) ||
                (!hard && (iswalpha(wtext[i]) ||
                           iswdigit(wtext[i]) ||
                           wtext[i] == '.' ||
                           wtext[i] == ':' ||
                           wtext[i] == '/' ||
                           wtext[i] == '\\')))
        {
            current += towlower(wtext[i]);
        }
        if (wtext[i] == ' ' || i == wtext.length() - 1) {
            if (!current.empty()) {
                if (!result.empty()) {
                    result += ' ';
                }
                result += current;
                current.clear();
            }
        }
    }
    return WideToUTF8(result);
}

string ImproveText(const string& text) {
    wstring wtext = UTF8ToWide(text);
    wstring current;
    wstring result;

    for (size_t i = 0; i < wtext.size(); i++) {
        if (iswspace(wtext[i])) {
            wtext[i] = ' ';
        } else {
            current += wtext[i];
        }
        if (wtext[i] == ' ' || i == wtext.length() - 1) {
            if (!current.empty() &&
                    current[0] != '.' &&
                    current[0] != ',' &&
                    current[0] != '!' &&
                    current[0] != '?')
            {
                if (!result.empty()) {
                    result += ' ';
                }
            }
            result += current;
            current.clear();
        }
    }
    if (iswupper(result[0]) && !iswpunct(result[result.size() - 1])) {
        result += '.';
    }
    boost::algorithm::trim(result);
    return WideToUTF8(result);
}

size_t CalcSentencesCount(const string& text) {
    if (text.size() == 0) {
        return 0;
    }
    size_t count = 0;
    for (size_t i = 1; i < text.size(); i++) {
        if (issentbreak(text[i]) && !ispunct(text[i - 1])) {
            count++;
        }
    }
    if (!issentbreak(text[text.size() - 1])) {
        count++;
    }
    return count > 0 ? count : (text.size() > 0 ? 1 : 0);
}
