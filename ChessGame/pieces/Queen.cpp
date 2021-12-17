
#include "Queen.h"
#include "../Game.h"
#include <iostream>

Queen::Queen(Player &player, Game &game, int x, int y) : Piece(player, game, x, y)
{
    //Constructor for Queen Class
    points = 9;
    //Adds piece to player's list of pieces
    player.pieces[player.num_pieces++] = this;
}

Queen::Queen(Player &player, Game &game, int x, int y, int pos) : Piece(player, game, x, y)
{
    //Constructor for Queen Class when pawn is promoted to Queen on 8th tile
    points = 9;
    //Adds a queen piece to location in pieces array, where promoted pawn was
    player.pieces[pos] = this;
}

Piece::Type Queen::get_type() const
{
    //Returns the type of current piece i.e. Queen
    return Piece::Type::QUEEN;
}

int Queen::get_point() const
{
    //Returns no. of points player receives upon capturing Queen.
    return points;
}

int Queen::list_possible_moves() // Function to compute the current Queen piece's possible moves
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

    //Multiple while loops to find current Queen's all possible moves and add them to possible_moves array as 2's

    int xpos = x;
    int ypos = y - 1;
    //Looping through all leftward moves
    while (true)
    {
        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        ypos--;
    }

    xpos = x;
    ypos = y + 1;
    //Looping through all rightward moves
    while (true)
    {

        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        ypos++;
    }

    xpos = x + 1;
    ypos = y;
    //Looping through all downward moves
    while (true)
    {
        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        xpos++;
    }

    xpos = x - 1;
    ypos = y;
    //Looping through all upward moves
    while (true)
    {
        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        xpos--;
    }

    xpos = x + 1;
    ypos = y + 1;
    //Looping through all bottom right diagonal moves
    while (true)
    {
        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        ypos++;
        xpos++;
    }

    xpos = x - 1;
    ypos = y + 1;
    //Looping through all top right diagonal moves
    while (true)
    {
        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        ypos++;
        xpos--;
    }

    xpos = x - 1;
    ypos = y - 1;
    //Looping through all top left diagonal moves
    while (true)
    {
        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        ypos--;
        xpos--;
    }

    xpos = x + 1;
    ypos = y - 1;
    //Looping through all bottom left diagonal moves
    while (true)
    {
        if (game.in_boundaries(xpos, ypos))
        {
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                if (game.get_piece(xpos, ypos)->which_player() != this->which_player())
                {
                    possible_moves[xpos][ypos] = 2;
                    num_moves++;
                }
                break;
            }
        }
        else
        {
            break;
        }
        ypos--;
        xpos++;
    }

    return num_moves;
}

void Queen::convert()
{
    //Sets converted to true if current Queen is a promoted pawn
    converted = true;
}

std::string Queen::get_img() const
{
    //Returns directory of image asset for piece
    if (!player.is_in_game())
        //Returns gray piece if player is deactivated (checkmate, stalemate, or disconnection)
        return ":/img/DeactivatedPlayer/queen.png";
    return ":/img/Player" + std::to_string(player.get_index()) + "/queen.png";
}
