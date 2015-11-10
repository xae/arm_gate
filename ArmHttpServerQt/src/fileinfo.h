#ifndef FILEINFO_H
#define FILEINFO_H

#include <QDateTime>
#include <QMetaType>

struct FileInfo {
    QString hash;
    QString filename;
    QString filepath;
    int ttl;
    QDateTime registeredTime;
};

#endif // FILEINFO_H
