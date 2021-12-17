
#include "Pawn.h"
#include "../Game.h"
#include "iostream"

Pawn::Pawn(Player &player, Game &game, int x, int y) : Piece(player, game, x, y)
{
    //Constructor for Pawn Class
    points = 1;
    //Adds piece to player's list of pieces
    player.pieces[player.num_pieces++] = this;
}

Piece::Type Pawn::get_type() const
{
    //Returns the type of current piece i.e. Pawn
    return Piece::Type::PAWN;
}

int Pawn::get_point() const
{
    //Returns no. of points player receives upon capturing Pawn
    return points;
}

int Pawn::list_possible_moves() // Function to compute the current Pawn piece's possible moves
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

    //if Player 1 - decrease x
    if (which_player() == 0)
    {
        if (!get_has_moved())
        {
            //Can move up 2
            if (game.in_boundaries(x - 1, y) && game.get_piece((x - 1), y) == nullptr)
            {
                //add it to arr
                possible_moves[x - 1][y] = 2;
                num_moves++;
                if (game.in_boundaries(x - 2, y) && game.get_piece((x - 2), y) == nullptr)
                {
                    //add it to arr
                    possible_moves[x - 2][y] = 2;
                    num_moves++;
                }
            }
            if (game.in_boundaries(x - 1, y + 1) && game.get_piece((x - 1), y + 1) != nullptr &&
                game.get_piece(x - 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y + 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x - 1, y - 1) && game.get_piece((x - 1), y - 1) != nullptr &&
                game.get_piece(x - 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y - 1] = 2;
                num_moves++;
            }
        }
        else
        {
            //Can only move up one
            if (game.in_boundaries(x - 1, y) && game.get_piece((x - 1), y) == nullptr)
            {
                //add it to arr
                possible_moves[x - 1][y] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x - 1, y + 1) && game.get_piece((x - 1), y + 1) != nullptr &&
                game.get_piece(x - 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y + 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x - 1, y - 1) && game.get_piece((x - 1), y - 1) != nullptr &&
                game.get_piece(x - 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y - 1] = 2;
                num_moves++;
            }
        }
    }
    //if Player 2 - increase Y
    if (which_player() == 1)
    {
        if (!get_has_moved())
        {
            //Can move up 2
            if (game.in_boundaries(x, y + 1) && game.get_piece((x), y + 1) == nullptr)
            {
                //add it to arr
                possible_moves[x][y + 1] = 2;
                num_moves++;
                if (game.in_boundaries(x, y + 2) && game.get_piece((x), y + 2) == nullptr)
                {
                    //add it to arr
                    possible_moves[x][y + 2] = 2;
                    num_moves++;
                }
            }
            if (game.in_boundaries(x + 1, y + 1) && game.get_piece((x + 1), y + 1) != nullptr &&
                game.get_piece(x + 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y + 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x - 1, y + 1) && game.get_piece((x - 1), y + 1) != nullptr &&
                game.get_piece(x - 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y + 1] = 2;
                num_moves++;
            }
        }
        else
        {
            //Can only move up one
            if (game.in_boundaries(x, y + 1) && game.get_piece((x), y + 1) == nullptr)
            {
                //add it to arr
                possible_moves[x][y + 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x + 1, y + 1) && game.get_piece((x + 1), y + 1) != nullptr &&
                game.get_piece(x + 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y + 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x - 1, y + 1) && game.get_piece((x - 1), y + 1) != nullptr &&
                game.get_piece(x - 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y + 1] = 2;
                num_moves++;
            }
        }
    }

    //if Player 3 - increase X
    if (which_player() == 2)
    {
        if (!get_has_moved())
        {
            //Can move up 2
            if (game.in_boundaries(x + 1, y) && game.get_piece((x + 1), y) == nullptr)
            {
                //add it to arr
                possible_moves[x + 1][y] = 2;
                num_moves++;
                if (game.in_boundaries(x + 2, y) && game.get_piece((x + 2), y) == nullptr)
                {
                    //add it to arr
                    possible_moves[x + 2][y] = 2;
                    num_moves++;
                }
            }
            if (game.in_boundaries(x + 1, y + 1) && game.get_piece((x + 1), y + 1) != nullptr &&
                game.get_piece(x + 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y + 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x + 1, y - 1) && game.get_piece((x + 1), y - 1) != nullptr &&
                game.get_piece(x + 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y - 1] = 2;
                num_moves++;
            }
        }
        else
        {
            //Can only move up one
            if (game.in_boundaries(x + 1, y) && game.get_piece((x + 1), y) == nullptr)
            {
                //add it to arr
                possible_moves[x + 1][y] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x + 1, y + 1) && game.get_piece((x + 1), y + 1) != nullptr &&
                game.get_piece(x + 1, y + 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y + 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x + 1, y - 1) && game.get_piece((x + 1), y - 1) != nullptr &&
                game.get_piece(x + 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y - 1] = 2;
                num_moves++;
            }
        }
    }

    //if Player 4 - decrease Y
    if (which_player() == 3)
    {
        if (!get_has_moved())
        {
            //Can move up 2
            if (game.in_boundaries(x, y - 1) && game.get_piece((x), y - 1) == nullptr)
            {
                //add it to arr
                possible_moves[x][y - 1] = 2;
                num_moves++;
                if (game.in_boundaries(x, y - 2) && game.get_piece((x), y - 2) == nullptr)
                {
                    //add it to arr
                    possible_moves[x][y - 2] = 2;
                    num_moves++;
                }
            }
            if (game.in_boundaries(x + 1, y - 1) && game.get_piece((x + 1), y - 1) != nullptr &&
                game.get_piece(x + 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y - 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x - 1, y - 1) && game.get_piece((x - 1), y - 1) != nullptr &&
                game.get_piece(x - 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y - 1] = 2;
                num_moves++;
            }
        }
        else
        {
            //Can only move up one
            if (game.in_boundaries(x, y - 1) && game.get_piece((x), y - 1) == nullptr)
            {
                //add it to arr
                possible_moves[x][y - 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x + 1, y - 1) && game.get_piece((x + 1), y - 1) != nullptr &&
                game.get_piece(x + 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x + 1][y - 1] = 2;
                num_moves++;
            }
            if (game.in_boundaries(x - 1, y - 1) && game.get_piece((x - 1), y - 1) != nullptr &&
                game.get_piece(x - 1, y - 1)->which_player() != which_player())
            {
                //add it to arr
                possible_moves[x - 1][y - 1] = 2;
                num_moves++;
            }
        }
    }

    return num_moves;
}

std::string Pawn::get_img() const
{
    //Returns directory of image asset for piece
    if (!player.is_in_game())
        //Returns gray piece if player is deactivated (checkmate, stalemate, or disconnection)
        return ":/img/DeactivatedPlayer/pawn.png";
    return ":/img/Player" + std::to_string(player.get_index()) + "/pawn.png";
}
