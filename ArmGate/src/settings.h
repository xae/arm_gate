#pragma once

#include <QObject>
#include <QVariant>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Settings);
    Settings();
    ~Settings();
    static Settings& instance();
public:
    static QVariant value(const QString& key,
        const QVariant& defaultValue = QVariant());
    static QSettings& get();

private:
    QSettings settings_;
};

