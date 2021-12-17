// This class is responsible for displaying the Main Window which contains the Chess Board
// and all player details.
// This window is where the 4-player chess game is played.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include "chessclientobj.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class ChessClientObj;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(ChessClientObj *client = nullptr, int count = 0, bool online = false, bool firstOnline = true, QWidget *parent = nullptr);

    ~MainWindow();

    void update_scores(int player_1_score, int player_2_score, int player_3_score, int player_4_score);

    void update_status(std::string player_1_status, std::string player_2_status, std::string player_3_status, std::string player_4_status);

    void set_bot_notif(bool player_2_bot, bool player_3_bot, bool player_4_bot);

private:
    Ui::MainWindow *ui;

    void initGrid();
};

#endif // MAINWINDOW_H
