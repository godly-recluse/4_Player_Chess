
#include "menuwindow.h"
#include "ui_menuwindow.h"
#include "mainwindow.h"
#include <iostream>
#include "qinputdialog.h"
#include "qmessagebox.h"
#include "unistd.h"

MenuWindow::MenuWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MenuWindow)
{
    //Constructor to create Menu Window upon starting game.
    ui->setupUi(this);
    if (connect == nullptr)
    {
        connect = new ChessClientObj();
    }
}

MenuWindow::~MenuWindow()
{
    //Handles closing of window and closes connection upon window close.
    delete ui;
    delete connect;
}

void MenuWindow::on_start_game_clicked()
{
    //Function to handle creation of local game.
    QInputDialog *inputDialog = new QInputDialog(this);

    //Input to determine number of bots
    QStringList items({"0", "1", "2", "3"});
    int bots = inputDialog->getItem(this, "Multiplayer Setup", "How many bots do you want to have?", items).toInt();

    //Sets up MenuWindow with online parameter set to false, as it is a Local game.
    MainWindow *m = new MainWindow(nullptr, bots, false, true);
    m->setAttribute(Qt::WA_DeleteOnClose);
    m->show();
    close();
    delete inputDialog;
}

void MenuWindow::on_create_game_clicked()
{
    //Function to handle creation of LAN Multiplayer game
    QInputDialog *inputDialog = new QInputDialog(this);
    QString ip = "";
    int port = -1;
    ip = inputDialog->getText(this, "Multiplayer Server", "Please enter the IP address of the server");
    if (ip != "")
    {
        port = inputDialog->getInt(this, "Multiplayer Setup", "Please enter the port number of the server");
    }
    if (ip != "" && port > 0)
    {
        // Tests connection based on IP Address and Port entered.
        bool test = connect->setupSocket(ip, port);
        if (!test)
        {
            //Displays error message if test not successful
            QMessageBox box(QMessageBox::Warning, "ERROR", "The server connection does not work. Are you sure you are putting in the correct address?");
            box.exec();
            delete inputDialog;
        }
        else
        {
            //If test connection succesful.

            //Stores number of players/clients connected to the game/server.
            int playerCount = connect->getCount();

            MainWindow *m = nullptr;
            int reg = 1;

            if (playerCount == 0) //If current player is the client setting up the game.
            {
                inputDialog->setIntMaximum(3);
                inputDialog->setIntMinimum(0);
                QStringList items({"0", "1", "2"});
                int bot = inputDialog->getItem(this, "Multiplayer Setup", "How many bots do you want to have?", items).toInt();
                m = new MainWindow(connect, bot, true, true);
                connect->registerMainWindow(m);
                connect->registerMenuWindow(this);
                reg = bot + 1;
            }

            //
            else //If current player is joining an existing game
            {
                m = new MainWindow(connect, connect->getCount(), true, false);
                connect->registerMainWindow(m);
                connect->registerMenuWindow(this);
            }

            //Waits before all players join, to show the Main Window and the Chess Board
            QMessageBox box(QMessageBox::Information, "Waiting", "Waiting for everyone to join the server. Please wait patiently. If you have to exit, please use cmd-alt-delete on Windows or force-quit on Mac");
            box.setStandardButtons(QMessageBox::Cancel);

            connect->registerWaitingPopup(&box);

            sleep(1);
            connect->preRegister(reg);

            box.exec();

            m->setAttribute(Qt::WA_DeleteOnClose);
        }
    }
}
