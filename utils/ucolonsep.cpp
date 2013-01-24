#include <QFile>
#include <QTextStream>
#include <QStringList>

#include "uexception.h"
#include "ucolonsep.h"

UColonSep::UColonSep(QObject *parent)
    : QObject(parent)
{
}

void UColonSep::Load(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw UException("Error opening file " + file.fileName());
    }
    QTextStream in(&file);
    QString line = "";
    while (!line.isNull()) {
        line = in.readLine();
        if (line.isEmpty() || line[0] == '#') {
            continue;
        }
        QStringList objParams = line.split(":");
        QVector<UFromStringFormat> objs;
        for (int i = 0; i < objParams.size(); i++) {
            objs.push_back(objParams[i]);
        }
        Objects.push_back(objs);
    }
}

size_t UColonSep::Rows() {
    return Objects.size();
}

size_t UColonSep::Cols(size_t rowNum) {
    if (rowNum > (size_t)Objects.size()) {
        throw UException("Index out of bounds");
    }
    return Objects[rowNum].size();
}

UFromStringFormat UColonSep::Get(size_t row, size_t col) {
    if (row > (size_t)Objects.size()
            || col > (size_t)Objects[row].size())
    {
        throw UException("Index out of bounds");
    }
    return Objects[row][col];
}
