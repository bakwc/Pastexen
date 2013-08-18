#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QMap>

#include "ucast.h"

class USettings: public QObject
{
    Q_OBJECT
public:
    typedef QHash<QString, QString> TParametersHash;

public:
    USettings(QString path = "", QObject* parent = 0);
    virtual ~USettings() {}
    void Load(const QString& fname, const QStringList& required = QStringList());
    void Save(const QString& fname, bool keepOrigin = false);

    void Save(bool keepOrigin = false);

    UFromStringFormat GetParameter(const QString& parameter);
    UFromStringFormat GetParameter(const QString &parameter, const QString& value);
    void SetParameter(const QString& parameter, const QString& value);
    void DefineParams(const TParametersHash& paramsList);

private:
    TParametersHash Parameters;
    QString Path;
};
