// This class is reponsible for setting up the behavior and flow of the Main Window (containing the Chess Grid).

#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QSignalMapper>
#include "Game.h"
#include "chessclientobj.h"
#include "mainwindow.h"

class ChessClientObj;

class MainWindow;

class ChessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChessWidget(ChessClientObj *client, int botCount = 0, bool online = false, bool firstOnline = true, MainWindow *mw = nullptr, QWidget *parent = nullptr);

    void resetColors(bool);

    void processPossibleBots();

private:
    int chosenx = 0;
    int choseny = 0;
    std::vector<Piece::Coordinates> current_piece_vec;
    Game *game;
    MainWindow *mw;

    int heightForWidth(int) const;

    bool offline;
    QPushButton *button_arr[14][14];

    void initMap(QGridLayout *, QSignalMapper *, Game *);

    void handle_valid_moves(Piece::Coordinates) const;

    bool check_piece(Piece::Coordinates);

private slots:
    void handleButtonClick(int);
};

#endif // CHESSWIDGET_H
