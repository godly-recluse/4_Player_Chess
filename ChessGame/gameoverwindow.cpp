#include "gameoverwindow.h"
#include "ui_gameoverwindow.h"
#include "menuwindow.h"
#include "Game.h"

GameOverWindow::GameOverWindow(bool end_to_disconnect, int winner_index, int winner_index_2, int winner_index_3, int winner_index_4, QWidget *parent) : QDialog(parent), ui(new Ui::GameOverWindow)
{
    //To display window after game ends
    ui->setupUi(this);

    if (!end_to_disconnect) // If game ends with one or more winners
    {
        if (winner_index_2 == 0) // If one player wins
        {
            ui->index_label->setText("Player " + QString::number(winner_index) + " Wins!");
            switch (winner_index - 1)
            {
            case 0:
                this->setStyleSheet("background-color: rgb(85, 126, 250);");
                break;

            case 1:
                this->setStyleSheet("background-color: rgb(255, 219, 111);");
                break;

            case 2:
                this->setStyleSheet("background-color: rgb(39, 174, 96);");
                break;

            case 3:
                this->setStyleSheet("background-color: rgb(235, 87, 87);");
                break;
            }
        }
        else if (winner_index_3 == 0) // If two players win (draw)
        {
            ui->index_label->setText("Player " + QString::number(winner_index) + " & Player " + QString::number(winner_index_2) + " Win!");
            this->setStyleSheet("background-color: rgb(220, 156, 250);");
        }
        else if (winner_index_4 == 0)// If three players win (draw)
        {
            ui->index_label->setText("Player " + QString::number(winner_index) + ", Player " + QString::number(winner_index_2));
            ui->index_label_2->setText("& Player " + QString::number(winner_index_3) + " Win!");
            this->setStyleSheet("background-color: rgb(220, 156, 250);");
        }
        else // If all players win (due to stalemate and same scores)
        {
            ui->index_label->setText("All Players Win!");
            this->setStyleSheet("background-color: rgb(220, 156, 250);");
        }
    }
    else //If game ends due to players getting disconnected in LAN version
    {
        ui->index_label->setText("All players have been disconnected");
        this->setStyleSheet("background-color: rgb(210, 210, 210);");
    }
    ui->index_label->setStyleSheet("font: 48pt \"Arial\"; font-weight: bold; qproperty-alignment: AlignCenter");
    ui->index_label_2->setStyleSheet("font: 48pt \"Arial\"; font-weight: bold; qproperty-alignment: AlignCenter");
}

GameOverWindow::~GameOverWindow()
{
    //To handle window close
    delete ui;
}

void GameOverWindow::on_quit_game_button_clicked()
{
    //To handle quit game button being clicked
    this->close();
}
