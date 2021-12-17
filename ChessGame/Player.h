// This class contains each Player's attributes which is essential to determine the status of the game.
// Its data members define whether the player is a bot or not, whether the player is online or not, whether the player is still in the game or not, among others.
// The string status data member also defines whether it is the Player's turn or not and whether the player is Checked, Checkmated or Stalemated.

#ifndef INC_4PCHESS_PLAYER_H
#define INC_4PCHESS_PLAYER_H

#include <string>

class Game;

class Piece;

class Player
{
public:
    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,

        DIRECTION
    };

    Player(Game &game);
    ~Player();

    void set_index(int index);
    int get_index();

    bool is_in_game();

    int get_score();
    void increase_score(int);

    void deactivate();

    bool get_is_bot() const;
    void set_is_bot();

    bool get_is_online() const;
    void set_is_online();

    std::string get_status();
    void set_status(std::string);

    int num_pieces = 0;
    Piece *pieces[16];

private:
    bool is_active = true;
    int score = 0;
    int indexNum = 0;
    Game &game;
    bool is_bot = false;
    bool is_online = false;
    std::string status = "Waiting...";
};

#endif //INC_4PCHESS_PLAYER_H
