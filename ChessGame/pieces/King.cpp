
#include "Piece.h"
#include <iostream>
#include "../Game.h"

King::King(Player &player, Game &game, int x, int y) : Piece(player, game, x, y)
{
    //Constructor for King Class
    points = 20;
    //Adds piece to player's list of pieces
    player.pieces[player.num_pieces++] = this;
}

Piece::Type King::get_type() const
{
    //Returns the type of current piece i.e. King
    return Piece::Type::KING;
}

int King::get_point() const
{
    //Returns no. of points player receives upon checkmating King.
    return points;
}

int King::list_possible_moves() // Function to compute the current King piece's possible moves
{
    //Initializing possible_moves array to 0
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {
            possible_moves[i][j] = 0;
        }
    }

    int num_moves = 0;

    //Checking all board tiles surrounding King and adding to possible_moves array if not occupied by current player's piece
    for (int i = x - 1; i < x + 2; ++i)
    {
        for (int j = y - 1; j < y + 2; ++j)
        {
            if (game.in_boundaries(i, j) == false)
                continue;

            if (game.get_piece(i, j) != nullptr)
            {
                if (game.get_piece(i, j)->which_player() != player.get_index())
                {
                    possible_moves[i][j] = 2;
                    num_moves++;
                }
            }
            else
            {
                possible_moves[i][j] = 2;
                num_moves++;
            }
        }
    }

    //Handles Castling
    int board_copy[14][14];
    game.create_board_copy(which_player(), board_copy);
    int index = which_player();

    if (!has_moved)
    {
        if (!game.is_checked(index, board_copy))
        {
            // Castling for Player 1
            if (index == 0)
            {
                if (game.get_piece(x, y + 3) != nullptr && !game.get_piece(x, y + 3)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x][y + 1] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x, y + 1) == nullptr && game.get_piece(x, y + 2) == nullptr)
                        {
                            possible_moves[x][y + 2] = 2;
                            num_moves++;
                        }
                    }
                }
                if (game.get_piece(x, y - 4) != nullptr && !game.get_piece(x, y - 4)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x][y - 1] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x, y - 1) == nullptr && game.get_piece(x, y - 2) == nullptr && game.get_piece(x, y - 3) == nullptr)
                        {
                            possible_moves[x][y - 2] = 2;
                            num_moves++;
                        }
                    }
                }
            }

            // Castling for Player 2
            else if (index == 1)
            {
                if (game.get_piece(x - 3, y) != nullptr && !game.get_piece(x - 3, y)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x - 1][y] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x - 1, y) == nullptr && game.get_piece(x - 2, y) == nullptr)
                        {
                            possible_moves[x - 2][y] = 2;
                            num_moves++;
                        }
                    }
                }
                if (game.get_piece(x + 4, y) != nullptr && !game.get_piece(x + 4, y)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x + 1][y] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x + 1, y) == nullptr && game.get_piece(x + 2, y) == nullptr && game.get_piece(x + 3, y) == nullptr)
                        {
                            possible_moves[x + 2][y] = 2;
                            num_moves++;
                        }
                    }
                }
            }

            // Castling for Player 3
            else if (index == 2)
            {
                if (game.get_piece(x, y - 3) != nullptr && !game.get_piece(x, y - 3)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x][y - 1] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x, y - 1) == nullptr && game.get_piece(x, y - 2) == nullptr)
                        {
                            possible_moves[x][y - 2] = 2;
                            num_moves++;
                        }
                    }
                }

                if (game.get_piece(x, y + 4) != nullptr && !game.get_piece(x, y + 4)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x][y + 1] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x, y + 1) == nullptr && game.get_piece(x, y + 2) == nullptr && game.get_piece(x, y + 3) == nullptr)
                        {
                            possible_moves[x][y + 2] = 2;
                            num_moves++;
                        }
                    }
                }
            }

            // Castling for Player 4
            else
            {
                if (game.get_piece(x + 3, y) != nullptr && !game.get_piece(x + 3, y)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x + 1][y] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x + 1, y) == nullptr && game.get_piece(x + 2, y) == nullptr)
                        {
                            possible_moves[x + 2][y] = 2;
                            num_moves++;
                        }
                    }
                }

                if (game.get_piece(x - 4, y) != nullptr && !game.get_piece(x - 4, y)->get_has_moved())
                {
                    int board_copy_castle[14][14];
                    game.create_board_copy(which_player(), board_copy_castle);
                    board_copy_castle[x - 1][y] = board_copy_castle[x][y];
                    board_copy_castle[x][y] = 0;

                    if (!game.is_checked(index, board_copy_castle))
                    {
                        if (game.get_piece(x - 1, y) == nullptr && game.get_piece(x - 2, y) == nullptr && game.get_piece(x - 3, y) == nullptr)
                        {
                            possible_moves[x - 2][y] = 2;
                            num_moves++;
                        }
                    }
                }
            }
        }
    }

    //Setting current piece to 1 in possible_moves array
    possible_moves[x][y] = 1;

    return num_moves;
}

std::string King::get_img() const
{
    //Returns directory of image asset for piece
    if (!player.is_in_game())
        //Returns gray piece if player is deactivated (checkmate, stalemate, or disconnection)
        return ":/img/DeactivatedPlayer/king.png";
    return ":/img/Player" + std::to_string(player.get_index()) + "/king.png";
}
