
#include "Piece.h"
#include "../Game.h"

Piece::Piece(Player &player, Game &game, int x, int y) : player{player}, game{game}, x(x), y(y), is_captured(false),
                                                         has_moved(false) {}

int Piece::which_player() { return player.get_index(); }

void Piece::setCoordinates(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Piece::set_is_captured()
{
    is_captured = true;
}

bool Piece::get_is_captured()
{
    return is_captured;
}
void Piece::set_points(int points)
{
    this->points = points;
}

void Piece::set_has_moved(bool moved)
{
    has_moved = moved;
}

bool Piece::get_has_moved()
{
    return has_moved;
}

std::vector<Piece::Coordinates> Piece::get_moves_vec()
{
    std::vector<Piece::Coordinates> ans;
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            if (possible_moves[i][j] == 2)
            {
                ans.push_back({i, j});
            }
        }
    }
    return ans;
}

int Piece::getX() { return x; }
int Piece::getY() { return y; }

void Piece::setX_0() { x = 0; }
void Piece::setY_0() { y = 0; }

int Piece::get_possible_move_at(int x, int y)
{
    return possible_moves[x][y];
}
void Piece::set_possible_move_at(int x, int y, int value)
{
    possible_moves[x][y] = value;
}
