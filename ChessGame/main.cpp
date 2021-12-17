
#include "mainwindow.h"
#include "menuwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    //To start Qt Chess Application
    QApplication a(argc, argv); // To create QApplication Object
    MenuWindow w; //Menu Window is displayed immediately upon start
    w.show();
    return a.exec();
}
