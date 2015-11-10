#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < 0x050000
    QTextCodec *cyrillicCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(cyrillicCodec);
#ifdef Q_OS_WIN
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Win-1251"));
#else
    QTextCodec::setCodecForLocale(cyrillicCodec);
#endif
    QTextCodec::setCodecForCStrings(cyrillicCodec);
#endif

    QApplication a(argc, argv);
    QApplication::setApplicationName("ARM Gate");
    QApplication::setWindowIcon(QIcon(":/new/list/LAN.png"));
    QApplication::setQuitOnLastWindowClosed(false);

    MainWindow w;

    return a.exec();
}
