
#include "chessclientobj.h"
#include "sstream"
#include "vector"
#include <QThread>
#include <random>

ChessClientObj::ChessClientObj(QObject *parent) : QObject(parent) {}

//Function to set up TCP Client Socket and establish connection with Server
//param address: IP Address of the server
//param port: port number of the server
//return true if connection is established, false if not.
bool ChessClientObj::setupSocket(QString address, int port)
{
    //Deregister current connections if it's not the first time running
    if (!first)
    {
        deregister();
    }
    qDebug() << "in SetupSocket";
    socket = new QTcpSocket(this); //creates TCP socket for Client-Server Connection
    socket->connectToHost(address, port); // connects to IP Address and Port no. as passed in function parameters
    if (socket->waitForConnected())
    {
        qDebug() << socket->localAddress() << " " << socket->localPort();
        //Receive the first message - information about currently connected to the server.
        if (socket->waitForReadyRead())
        {
            QString line = socket->readLine().trimmed();

            stringstream ss(line.toStdString());
            string command = "";
            ss >> command;

            //If-Else-If block that performs actions based on server feedback
            if (command == "info")
            {
                bool flag = false;
                while (!ss.eof())
                {
                    string x = "";
                    ss >> x;
                    std::cout << x << std::endl;
                    if (x == "other")
                    {
                        flag = true;
                    }
                    else if (x == "count")
                    {
                        ss >> numPlayers;
                    }
                    else
                    {
                        //botList is the list that tracks which index is a bot or not
                        //phylist is the list that tracks is the player is an online human player or not
                        //Indexes saved to the both lists
                        if (!flag)
                        {
                            botList.push_back(stoi(x));
                        }
                        else
                        {
                            phyList.push_back(stoi(x));
                        }
                    }
                }
            }
        }

        //Create a signature for message belonging verification
        signature = QString::number(Game::get_random_number(1, 10000)) + (QString::number(socket->localPort()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(receive())); //first player establishes connection
        first = false;
        return true;
    }
    else //if connection not made within given time
    {
        qDebug() << "Not a valid address";
        return false;
    }
}

//Sends a message to the server
//param msg - message to be sent
void ChessClientObj::send(QString msg)
{
    msg.append(" ");
    //Add the signature to the emssage
    msg.append(signature);
    msg.append("\t\n");
    qDebug() << msg;
    //Send it and wait for it to be sent
    socket->write(msg.toUtf8());
    socket->flush();
    socket->waitForBytesWritten();
    socket->flush();
    socket->waitForBytesWritten(1000);
}

//Receive slot for the QTcpSocket
void ChessClientObj::receive()
{
    qDebug() << "HERE";
    while (socket->canReadLine())
    {
        QString input = socket->readLine().trimmed();
        input.remove("\t\n");
        qDebug() << input;
        stringstream ss(input.toStdString());
        string command = "";
        ss >> command;

        // If the message is an info type (Redundant but held for debugging purposes)
        if (command == "info")
        {
            while (!ss.eof())
            {
                string x = "";
                ss >> x;
                if (x != "count")
                {
                    botList.push_back(stoi(x));
                }
                if (x == "count")
                {
                    ss >> numPlayers;
                }
            }
        }

        // Received when server reaches 4 players
        if (command == "START")
        {
            if (!started)
            {
                if (box != nullptr)
                {
                    qDebug() << "NOTNULL";
                    box->accept();
                }
                //Start main and close menu.
                menu->close();
                main->show();
                started = true;
            }
        }

        // If it's a move function
        if (command == "move")
        {

            int initX, initY, destX, destY;
            string sign;
            ss >> initX;
            ss >> initY;
            ss >> destX;
            ss >> destY;
            ss >> sign;

           // Check if the client sent this message is not the current client
            if (QString::fromStdString(sign) != signature)
            {
                game->movePiece(initX, initY, destX, destY);
                widget->resetColors(true);
                widget->processPossibleBots();
            }
        }

        // If a user disconnects from the server
        if (command == "dead")
        {
            int player;
            ss >> player;
            game->deactivate_player(player, "Disconnected");
            widget->resetColors(true);
            int active = game->active_players();
            qDebug() << active;

            //Check if there's only one person still in the game - if so, end the game.
            if (game->active_players() == 1)
            {
                game->end_game(true);
            }
        }
    }
}


void ChessClientObj::isConnected() const
{
    //Function to determine whether connection is successfully maintained.
    if (socket->isOpen())
    {
        qDebug() << "Connected";
    }
    else
        qDebug() << "Disconnected";
}

// Registers the players to the client (indicates if they are bot or not.
// Server structure indicates each computer will host its own version of the
// game and only the move commands will be exchanged.
void ChessClientObj::Register(QString index, bool isBot)
{

    if (isBot)
    {
        QString msg = "reg " + index + " true";
        send(msg);
    }
    else
    {
        QString msg = "reg " + index + " false";
        send(msg);
    }
}

// Send the move results to the server
void ChessClientObj::move(int initX, int initY, int destX, int destY)
{
    // Function to generate string that communicates actual player move to Server
    QString msg = "move " + QString::fromStdString(to_string(initX)) + " " + QString::fromStdString(to_string(initY)) + " " + QString::fromStdString(to_string(destX)) + " " + QString::fromStdString(to_string(destY));
    send(msg);
}

void ChessClientObj::moveBot(int initX, int initY, int destX, int destY)
{
    // Function to generate string that communicates bot move to Server
    QString msg = "move " + QString::fromStdString(to_string(initX)) + " " + QString::fromStdString(to_string(initY)) + " " + QString::fromStdString(to_string(destX)) + " " + QString::fromStdString(to_string(destY));
    send(msg);
}

//Get the number of players connected to the server
void ChessClientObj::getNum()
{
    send("num");
}

// Deregister all players from server, but keep the socket open
void ChessClientObj::deregister()
{
    send("deregister");
}

void ChessClientObj::flush()
{
    socket->flush();
}

// Pre-register the client (only # of users)
void ChessClientObj::preRegister(int count)
{
    send("prereg " + QString::fromStdString(to_string(count)));
}

// Register the MainWindow in the client so that we can start it from here when we receive "START"
void ChessClientObj::registerMainWindow(MainWindow *mw)
{
    main = mw;
}

// Register the Game in the client so that we can start it from here when we receive "START"
void ChessClientObj::registerGamePtr(Game *g)
{
    game = g;
}

// Register the chessWidget in the client so that we can start it from here when we receive "START"
void ChessClientObj::registerWidget(ChessWidget *cw)
{
    widget = cw;
}

// Register the menu window in the client so that we can close it from here when we receive "START"
void ChessClientObj::registerMenuWindow(MenuWindow *mw)
{
    menu = mw;
}

// Register the waiting popup in the client so that we can close it from here when we receive "START"
void ChessClientObj::registerWaitingPopup(QMessageBox *bx)
{
    qDebug() << "here";
    box = bx;
}

// Close the socket
void ChessClientObj::close()
{
    qDebug() << "here";
    socket->disconnect();
}

// Get the number of players already connected to the server
int ChessClientObj::getCount()
{
    return numPlayers;
}
