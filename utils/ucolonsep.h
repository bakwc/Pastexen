#pragma once

#include <QObject>
#include <QVector>

#include "ucast.h"

class UColonSep : public QObject {
    Q_OBJECT
public:
    UColonSep(QObject* parent = NULL);
    void Load(const QString& fileName);
    size_t Rows();
    size_t Cols(size_t rowNum);
    UFromStringFormat Get(size_t row, size_t col);
private:
    QVector<QVector<UFromStringFormat> > Objects;
};
