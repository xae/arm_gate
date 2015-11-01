#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    QApplication::setApplicationName("ARM Gate");
    QApplication::setWindowIcon(QIcon(":/new/list/LAN.png"));
    QApplication::setQuitOnLastWindowClosed(false);

    MainWindow w;

    return a.exec();
}
