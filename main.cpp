//#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    if(sizeof(int)<4)
        quitApp(ERRORINTTOOSHORT);
    QApplication a(argc, argv);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    MainWindow w;
    w.show();
    
    return a.exec();
}
