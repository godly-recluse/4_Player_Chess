
#include "chesswidget.h"
#include <QGridLayout>
#include <QLayout>
#include <QVBoxLayout>
#include "Game.h"
#include <QIcon>
#include <iostream>
#include <QSignalMapper>
#include "pieces/Piece.h"
#include <algorithm>
#include <vector>
#include "menuwindow.h"
#include <QApplication>
#include <QThread>
#include "unistd.h"

ChessWidget::ChessWidget(ChessClientObj *client, int botCount, bool online, bool firstOnline, MainWindow *mw, QWidget *parent) : QWidget(parent), mw(mw)
{
    //Constructor for Chess Widget Class
    game = new Game(client, botCount, online, firstOnline, mw);
    if (client != nullptr)
    {
        client->registerWidget(this);
    }

    //Sets a grid layout to prepare for creation of Chess Board
    QGridLayout *gridLayout = new QGridLayout(this);
    QSignalMapper *mapper = new QSignalMapper(this);

    //Calls initMap function to create a grid of QPushButtons, for the Chess Board
    initMap(gridLayout, mapper, game);

    QSizePolicy p = this->sizePolicy();
    p.setHeightForWidth(true);
    this->setSizePolicy(p);

    //Initially set player scores to 0 and displays it in the Main Window, using a pointer to the update_scores function.
    mw->update_scores(0, 0, 0, 0);
}

int ChessWidget::heightForWidth(int width) const
{
    return width;
}

void ChessWidget::initMap(QGridLayout *gridLayout, QSignalMapper *mapper, Game *game)
{
    //To set the spacing of buttons in the grid, to zero.
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);

    //Creating 14*14 grid of buttons for 4 player chess board, using nested-for loop
    for (int row = 0; row < 14; ++row)
    {
        for (int col = 0; col < 14; ++col)
        {
            //Create a button and map it to a value that's decodeable based on # rows and cols in the game
            QPushButton *button = new QPushButton(" ");
            mapper->setMapping(button, row * 14 + col);
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
            button->setFixedHeight(57);
            button->setFixedWidth(57);
            if ((row + col) % 2 == 0)
            {
                //Setting alternate tiles to have Black Background
                button->setStyleSheet("background-color: rgb(183, 192, 216); text-align:right");
            }
            else
            {
                //Other tiles set to White Background
                button->setStyleSheet("background-color: rgb(232, 237, 249); text-align:right");
            }
            if ((row < 3 && col < 3) || (row < 3 && col > 10) || (row > 10 && col < 3) || (row > 10 && col > 10))
            {
                //Disabling buttons at the corners, which are not part of the Chess Board
                button->setEnabled(false);
                button->setStyleSheet("background-color: silver; text-align: right");
            }
            if (game->get_piece(row, col) != nullptr)
            {
                //Adding the chess pieces as icons to the buttons in the Chess Board

                //Setting the initial layout of the board as stored in the game object of the Game Class
                button->setIcon(QIcon(QString::fromStdString(game->get_piece(row, col)->get_img())));
                button->setIconSize(QSize(45, 45));
            }

            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button_arr[row][col] = button;
            int player = game->get_local_player();
            qDebug() << player;

            //If-Else-If block set the board layout according to the current player's number/position.
            //Player has their own pieces immediately facing them.
            if (player == 0)
            {
                gridLayout->addWidget(button, row, col);
            }
            else if (player == 1)
            {
                gridLayout->addWidget(button, 14 - col, row);
            }
            else if (player == 2)
            {
                gridLayout->addWidget(button, 14 - row, 14 - col);
            }
            else if (player == 3)
            {
                gridLayout->addWidget(button, col, 14 - row);
            }
        }
    }

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(handleButtonClick(int)));

    //Updates status of the players in the Main Window using a pointer to the update_status function.
    mw->update_status(game->get_player_at(0)->get_status(), game->get_player_at(1)->get_status(), game->get_player_at(2)->get_status(), game->get_player_at(3)->get_status());
}

void ChessWidget::handleButtonClick(int index)
{

    if (chosenx == index / 14 && choseny == index % 14)
    {
        //Do nothing
        return;
    }

    //If the piece is the own piece
    if (game->get_piece(index / 14, index % 14) != nullptr &&
        ((!game->get_is_online() && game->get_piece(index / 14, index % 14)->which_player() == game->get_cur_player()) || (game->get_is_online() && game->get_piece(index / 14, index % 14)->which_player() == game->get_cur_player() && game->get_piece(index / 14, index % 14)->which_player() == game->get_online_player())))
    {
        //Assume you're choosing a piece of yourself
        resetColors(false);
        button_arr[index / 14][index % 14]->setStyleSheet("background-color: rgb(123, 97, 255); text-align: right");
        //Get all possible moves
        current_piece_vec = game->get_piece(index / 14, index % 14)->get_moves_vec();
        //Paint it using handle_valid_moves
        for_each(current_piece_vec.begin(), current_piece_vec.end(), [&](Piece::Coordinates p) { handle_valid_moves(p); });
        //Edit tracking locations
        chosenx = index / 14;
        choseny = index % 14;

        //DO
        return;
    }
    bool isarequest = false;

    //If the selected button matches any of the chosen piece's list
    for (unsigned long i = 0; i < current_piece_vec.size(); ++i)
    {
        if (current_piece_vec.at(i).x == index / 14 && current_piece_vec.at(i).y == index % 14)
        {
            isarequest = true;
        }
    }

    //If it does - user orders a movement
    if (isarequest)
    {
        game->movePiece(chosenx, choseny, index / 14, index % 14);
        //Update ui elements
        mw->update_scores(game->get_player_at(0)->get_score(), game->get_player_at(1)->get_score(), game->get_player_at(2)->get_score(), game->get_player_at(3)->get_score());
        //reset all variables
        chosenx = 0;
        choseny = 0;
        current_piece_vec.resize(0);
        resetColors(true);
        QApplication::processEvents();

        //Just check if the next players are bots. if so, do moves for those too
        while (game->get_cur_player_pointer()->get_is_bot() && !game->get_cur_player_pointer()->get_is_online())
        {
            game->bot_move_piece();
            mw->update_scores(game->get_player_at(0)->get_score(), game->get_player_at(1)->get_score(), game->get_player_at(2)->get_score(), game->get_player_at(3)->get_score());
            resetColors(true);
            QApplication::processEvents();
            sleep(1);
        }
    }
    else
    {
        //For debugging purposes
    }
}

// For debugging purposes
void ChessWidget::processPossibleBots()
{
    while (game->get_cur_player_pointer()->get_is_bot() && !game->get_cur_player_pointer()->get_is_online())
    {
        game->bot_move_piece();
        resetColors(true);
        QApplication::processEvents();
    }
}

//Reset colors of the buttons
//param icon - true if we need to update icons as well.
void ChessWidget::resetColors(bool icon)
{
    for (int row = 0; row < 14; ++row)
    {
        for (int col = 0; col < 14; ++col)
        {
            QPushButton *button = button_arr[row][col];
            if ((row + col) % 2 == 0)
            {
                button->setStyleSheet("background-color: rgb(183, 192, 216); text-align: right");
            }
            else
            {
                button->setStyleSheet("background-color: rgb(232, 237, 249) ; text-align: right");
            }
            if ((row < 3 && col < 3) || (row < 3 && col > 10) || (row > 10 && col < 3) || (row > 10 && col > 10))
            {
                button->setEnabled(false);
                button->setStyleSheet("background-color: silver; text-align: right");
            }

            if (icon && game->get_piece(row, col) != nullptr)
            {
                button->setIcon(QIcon(QString::fromStdString(game->get_piece(row, col)->get_img())));
                button->setIconSize(QSize(45, 45));
            }
            else if (icon && game->get_piece(row, col) == nullptr)
            {
                button->setIcon(QIcon());
            }
        }
    }
}

//Change the background to a certain color
void ChessWidget::handle_valid_moves(Piece::Coordinates p) const
{
    button_arr[p.x][p.y]->setStyleSheet("background-color: rgb(178, 167, 252); text-align: right");
}

//Check if a piece matches with the chosen piece.
bool ChessWidget::check_piece(Piece::Coordinates p)
{
    if (p.x == chosenx && p.y == choseny)
    {
        return true;
    }
    return false;
}
