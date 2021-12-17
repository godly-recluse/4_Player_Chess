
#include "Knight.h"
#include "iostream"
#include "../Game.h"

Knight::Knight(Player &player, Game &game, int x, int y) : Piece(player, game, x, y)
{
    //Constructor for Knight Class
    points = 3;
    //Adds piece to player's list of pieces
    player.pieces[player.num_pieces++] = this;
}

Piece::Type Knight::get_type() const
{
    //Returns the type of current piece i.e. Knight
    return Piece::Type::KNIGHT;
}

int Knight::get_point() const
{
    //Returns no. of points player receives upon capturing King.
    return points;
}

int Knight::list_possible_moves() // Function to compute the current Knight piece's possible moves
{
    //Initializing possible_moves array to 0
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {
            possible_moves[i][j] = 0;
        }
    }

    //Setting current piece to 1 in possible_moves array
    possible_moves[x][y] = 1;
    int num_moves = 0;

    //If-Else-If block to find all current Knight's possible moves and add them to possible_moves array as 2's

    //Checking 2 down and 1 right
    if (game.in_boundaries(x + 2, y + 1))
    {
        if (game.get_piece(x + 2, y + 1) != nullptr)
        {
            if (game.get_piece(x + 2, y + 1)->which_player() != player.get_index())
            {
                possible_moves[x + 2][y + 1] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x + 2][y + 1] = 2;
            num_moves++;
        }
    }

    //Checking 2 down and 1 left
    if (game.in_boundaries(x + 2, y - 1))
    {
        if (game.get_piece(x + 2, y - 1) != nullptr)
        {
            if (game.get_piece(x + 2, y - 1)->which_player() != player.get_index())
            {
                possible_moves[x + 2][y - 1] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x + 2][y - 1] = 2;
            num_moves++;
        }
    }

    //Checking 2 up and 1 right
    if (game.in_boundaries(x - 2, y + 1))
    {
        if (game.get_piece(x - 2, y + 1) != nullptr)
        {
            if (game.get_piece(x - 2, y + 1)->which_player() != player.get_index())
            {
                possible_moves[x - 2][y + 1] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x - 2][y + 1] = 2;
            num_moves++;
        }
    }

    //Checking 2 up and 1 left
    if (game.in_boundaries(x - 2, y - 1))
    {
        if (game.get_piece(x - 2, y - 1) != nullptr)
        {
            if (game.get_piece(x - 2, y - 1)->which_player() != player.get_index())
            {
                possible_moves[x - 2][y - 1] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x - 2][y - 1] = 2;
            num_moves++;
        }
    }

    //Checking 2 right and 1 down
    if (game.in_boundaries(x + 1, y + 2))
    {
        if (game.get_piece(x + 1, y + 2) != nullptr)
        {
            if (game.get_piece(x + 1, y + 2)->which_player() != player.get_index())
            {
                possible_moves[x + 1][y + 2] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x + 1][y + 2] = 2;
            num_moves++;
        }
    }

    //Checking 2 right and 1 up
    if (game.in_boundaries(x - 1, y + 2))
    {
        if (game.get_piece(x - 1, y + 2) != nullptr)
        {
            if (game.get_piece(x - 1, y + 2)->which_player() != player.get_index())
            {
                possible_moves[x - 1][y + 2] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x - 1][y + 2] = 2;
            num_moves++;
        }
    }

    //Checking 2 left and 1 down
    if (game.in_boundaries(x + 1, y - 2))
    {
        if (game.get_piece(x + 1, y - 2) != nullptr)
        {
            if (game.get_piece(x + 1, y - 2)->which_player() != player.get_index())
            {
                possible_moves[x + 1][y - 2] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x + 1][y - 2] = 2;
            num_moves++;
        }
    }

    //Checking 2 left and 1 up
    if (game.in_boundaries(x - 1, y - 2))
    {
        if (game.get_piece(x - 1, y - 2) != nullptr)
        {
            if (game.get_piece(x - 1, y - 2)->which_player() != player.get_index())
            {
                possible_moves[x - 1][y - 2] = 2;
                num_moves++;
            }
        }
        else
        {
            possible_moves[x - 1][y - 2] = 2;
            num_moves++;
        }
    }

    return num_moves;
}

std::string Knight::get_img() const
{
    //Returns directory of image asset for piece
    if (!player.is_in_game())
        //Returns gray piece if player is deactivated (checkmate, stalemate, or disconnection)
        return ":/img/DeactivatedPlayer/knight.png";
    return ":/img/Player" + std::to_string(player.get_index()) + "/knight.png";
}
