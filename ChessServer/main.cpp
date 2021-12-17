
#include <QCoreApplication>
#include "chessserverobj.h"

int main(int argc, char *argv[])
{
    //To start Qt Server Application
    QCoreApplication a(argc, argv);
    ChessServerObj server;
    return a.exec();
}
