#include "settings.h"
#include <QApplication>

//-----------------------------------------//
Settings::Settings() :
    settings_(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)
{
    settings_.sync();
}
//-----------------------------------------//
Settings::~Settings()
{
    settings_.sync();
}
//-----------------------------------------//
/*static*/ Settings &Settings::instance()
{
    static Settings s_instance;
    return s_instance;
}
//-----------------------------------------//
QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    if(!instance().settings_.contains(key) && !defaultValue.isNull())
        instance().settings_.setValue(key, defaultValue);
    return instance().settings_.value(key, defaultValue);
}
//-----------------------------------------//
QSettings &Settings::get()
{
    return instance().settings_;
}
//-----------------------------------------//
