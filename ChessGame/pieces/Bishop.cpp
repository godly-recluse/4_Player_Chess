
#include "Bishop.h"
#include "../Game.h"
#include "Piece.h"
#include "iostream"

Bishop::Bishop(Player &player, Game &game, int x, int y) : Piece(player, game, x, y)
{
    //Constructor for Bishop Class
    points = 5;
    //Adds piece to player's list of pieces
    player.pieces[player.num_pieces++] = this;
}

Piece::Type Bishop::get_type() const
{
    //Returns the type of current piece i.e. Bishop
    return Piece::Type::BISHOP;
}

int Bishop::get_point() const
{
    //Returns no. of points player receives upon capturing Bishop
    return points;
}

int Bishop::list_possible_moves() // Function to compute the current Bishop piece's possible moves
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

    //Initiliazes number of possible moves to 0
    int num_moves = 0;

    //Loop to generate bottom right diagonal moves for Bishop
    int xpos = x + 1;
    int ypos = y + 1;
    while (true)
    {
        //If move is within the boundaries of the board
        if (game.in_boundaries(xpos, ypos))
        {
            //If board tile is empty
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                //All possible moves set as as 2 in possible_moves array
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                //If board tile contains other player's piece
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

    //Loop to generate top right diagonal moves for Bishop
    xpos = x - 1;
    ypos = y + 1;
    while (true)
    {
        //If move is within the boundaries of the board
        if (game.in_boundaries(xpos, ypos))
        {
            //If board tile is empty
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                //All possible moves set as as 2 in possible_moves array
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                //If board tile contains other player's piece
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

    //Loop to generate top left diagonal moves for Bishop
    xpos = x - 1;
    ypos = y - 1;
    while (true)
    {
        //If move is within the boundaries of the board
        if (game.in_boundaries(xpos, ypos))
        {
            //If board tile is empty
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                //All possible moves set as as 2 in possible_moves array
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                //If board tile contains other player's piece
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

    //Loop to generate bottom left diagonal moves for Bishop
    xpos = x + 1;
    ypos = y - 1;
    while (true)
    {
        //If move is within the boundaries of the board
        if (game.in_boundaries(xpos, ypos))
        {
            //If board tile is empty
            if (game.get_piece(xpos, ypos) == nullptr)
            {
                //All possible moves set as as 2 in possible_moves array
                possible_moves[xpos][ypos] = 2;
                num_moves++;
            }
            else
            {
                //If board tile contains other player's piece
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

    //Returns the number of possible moves for current bishop piece
    return num_moves;
}

std::string Bishop::get_img() const
{
    //Returns directory of image asset for piece
    if (!player.is_in_game())
        //Returns gray piece if player is deactivated (checkmate, stalemate, or disconnection)
        return ":/img/DeactivatedPlayer/bishop.png";
    return ":/img/Player" + std::to_string(player.get_index()) + "/bishop.png";
}
