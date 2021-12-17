// This class is responsible for showing the Menu Window which is the first window,
// that the user encounters upon starting the game.
// It allows users to choose between starting local or LAN-based multiplayer games with anywhere from 1-3 bots.

#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include "chessclientobj.h"

namespace Ui
{
    class MenuWindow;
}

class MenuWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private:
    Ui::MenuWindow *ui;
    ChessClientObj *connect = nullptr;

private slots:
    void on_start_game_clicked();
    void on_create_game_clicked();
};

#endif // MENUWINDOW_H
