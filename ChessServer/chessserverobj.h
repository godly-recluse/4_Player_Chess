// This class is responsible for setting up the server for the LAN-multiplayer chess game.
// It handles events such as registering new clients/players, receiving and broacasting player moves, player disconnections, among others.

#ifndef CHESSSERVEROBJ_H
#define CHESSSERVEROBJ_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

using namespace std;

class ChessServerObj : public QObject
{
    Q_OBJECT
public:
    explicit ChessServerObj(QObject *parent = 0);

    void broadcast(QString str = "");

    void sendInfo(QTcpSocket *socket);

    void getPrevPlayersInfo(QTcpSocket *socket);

    void registerPlayer(QString player, QTcpSocket *socket, bool isBot);

    void broadcastExcept(QString str, QTcpSocket *except);

    void send(QString msg, QTcpSocket *socket);

    void broadcastRemoval(string s);

    void deregister(QTcpSocket *socket);

public slots:
    void newConnection();

    void onDisconnect();

    void onReceive();

private:
    struct player
    {
        QTcpSocket *socket;
        string player;
        bool isBot;
    };

    struct connection
    {
        QTcpSocket *socket;
        int count;
        void setCount(int c) { count = c; };
    };

    int count_players = 0;
    QTcpServer *server;
    map<QTcpSocket *, int> connectionlist;
    vector<player> playerList;
};

#endif // CHESSSERVEROBJ_H
