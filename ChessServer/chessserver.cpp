#include "chessserver.h"

ChessServer::ChessServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!server->listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Server didn't start";
    }
    else
    {
        qDebug() << "Server started";
    }
}

void ChessServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write(("hello client\r\n"));
    socket->flush();
    socket->waitForBytesWritten(3000);
    socket->close();
}
