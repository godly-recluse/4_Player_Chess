#ifndef CHESSSERVER_H
#define CHESSSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

class ChessServer : public QObject
{
    Q_OBJECT
public:
    explicit ChessServer(QObject *parent = 0);
signals:

public slots:
    void newConnection();

private:
    QTcpServer *server;
};

#endif // CHESSSERVER_H
