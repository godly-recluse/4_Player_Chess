

#include "Game.h"
#include "Player.h"
#include "pieces/Piece.h"
#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include "gameoverwindow.h"

namespace Random_Generator // Namespace for a global Pseudo-Random Number Generator to prevent naming collisions
{
    std::mt19937 mersenne{static_cast<std::mt19937::result_type>(std::time(nullptr))};
}

int Game::get_random_number(int min, int max) // Function to retrieve a random number within the range min-max, inclusive
{
    std::uniform_int_distribution<int> uniform_dist(min, max);
    return uniform_dist(Random_Generator::mersenne);
}

Game::Game(ChessClientObj *client, int botCount, bool online, bool firstOnline, MainWindow *mw) : is_online(online), client(client), mw(mw)
{ // The game constructor.
    turn_number = 0;

    // If game is online and current player is setting up/creating the game i.e. Player 1
    if (online && firstOnline)
    {
        // First player is always at index 0 i.e. Player 1
        for (int i = 0; i < 4; ++i)
        {
            players[i] = new Player(*this);
            players[i]->set_index(i);
        }
        client->Register(QString::fromStdString(to_string(0)), false);

        for (int i = 0; i < botCount; ++i)
        {
            //Randomly assign bots to different player indices/numbers.
            int index = get_random_number(1, 3);
            if (players[index]->get_is_bot())
            {
                --i;
                continue;
            }
            players[index]->set_is_bot();
            client->Register(QString::fromStdString(to_string(index)), true);
            client->flush();
        }

        //Determining which players are online
        local_player = 0;
        for (int i = 1; i < 4; ++i)
        {
            if (!players[i]->get_is_bot())
            {
                players[i]->set_is_online();
            }
        }
        client->registerGamePtr(this);
    }

    // If Game is online and client is joining already created game.
    else if (online && !firstOnline)
    {
        //Creating all players
        for (int i = 0; i < 4; ++i)
        {
            players[i] = new Player(*this);
            players[i]->set_index(i);
        }

        //Determining which players are bots based on data pulled from client
        for_each(client->botList.begin(), client->botList.end(), [&](int i){players[i]->set_is_bot();});

        int flag = 0;
        players[0]->set_is_online();

        //Setting which players are bots and which are actual players.
        for (int i = 1; i < 4; ++i)
        {
            if (players[i]->get_is_bot())
            {
                players[i]->set_is_online();
            }
            if (!players[i]->get_is_bot() && flag)
            {
                players[i]->set_is_online();
            }
            else if (!players[i]->get_is_bot() && !flag)
            {
                //This will be the first player to play.
                flag = i;
                local_player = i;
            }
        }

        client->Register(QString::fromStdString(to_string(flag)), false);
        client->flush();
        client->registerGamePtr(this);
    }

    // If it is a local game
    else
    {
        //Creating all players
        for (int i = 0; i < 4; ++i)
        {
            players[i] = new Player(*this);
            players[i]->set_index(i);
        }

        for (int i = 0; i < botCount; ++i)
        {
            //Randomly assign bots to different player indices/numbers.
            int index = get_random_number(1, 3);
            if (players[index]->get_is_bot())
            {
                --i;
                continue;
            }
            players[index]->set_is_bot();
        }
        local_player = 0;
    }

    //Creating initial chess board and setting pieces on the board according to their initial positions.
    for (int y = 0; y < 14; ++y)
    {
        for (int x = 0; x < 14; ++x)
        {
            board[x][y].piece = nullptr;

            if ((y < 3 && x < 3) || (y > 10 && x < 3) || (y > 10 && x > 10) || (y < 3 && x > 10))
            {
                //Deactivating the corner tiles, which are not part of the 4-player Chess Board.
                board[x][y].is_activated = false;
            }
            else if (y < 2)
            {
                if (y == 1)
                {
                    board[x][y].piece = new Pawn(*players[1], *this, x, y);
                }
                else
                {
                    if (x == 3 || x == 10)
                        board[x][y].piece = new Rook(*players[1], *this, x, y);
                    else if (x == 4 || x == 9)
                        board[x][y].piece = new Knight(*players[1], *this, x, y);
                    else if (x == 5 || x == 8)
                        board[x][y].piece = new Bishop(*players[1], *this, x, y);
                    else if (x == 7)
                        board[x][y].piece = new Queen(*players[1], *this, x, y);
                    else if (x == 6)
                        board[x][y].piece = new King(*players[1], *this, x, y);
                }
            }
            else if (x < 2)
            {
                if (x == 1)
                    board[x][y].piece = new Pawn(*players[2], *this, x, y);
                else
                {
                    if (y == 3 || y == 10)
                        board[x][y].piece = new Rook(*players[2], *this, x, y);
                    else if (y == 4 || y == 9)
                        board[x][y].piece = new Knight(*players[2], *this, x, y);
                    else if (y == 5 || y == 8)
                        board[x][y].piece = new Bishop(*players[2], *this, x, y);
                    else if (y == 7)
                        board[x][y].piece = new Queen(*players[2], *this, x, y);
                    else if (y == 6)
                        board[x][y].piece = new King(*players[2], *this, x, y);
                }
            }
            else if (y > 11)
            {
                if (y == 12)
                    board[x][y].piece = new Pawn(*players[3], *this, x, y);
                else
                {
                    if (x == 3 || x == 10)
                        board[x][y].piece = new Rook(*players[3], *this, x, y);
                    else if (x == 4 || x == 9)
                        board[x][y].piece = new Knight(*players[3], *this, x, y);
                    else if (x == 5 || x == 8)
                        board[x][y].piece = new Bishop(*players[3], *this, x, y);
                    else if (x == 7)
                        board[x][y].piece = new King(*players[3], *this, x, y);
                    else if (x == 6)
                        board[x][y].piece = new Queen(*players[3], *this, x, y);
                }
            }
            else if (x > 11)
            {
                if (x == 12)
                    board[x][y].piece = new Pawn(*players[0], *this, x, y);
                else
                {
                    if (y == 3 || y == 10)
                        board[x][y].piece = new Rook(*players[0], *this, x, y);
                    else if (y == 4 || y == 9)
                        board[x][y].piece = new Knight(*players[0], *this, x, y);
                    else if (y == 5 || y == 8)
                        board[x][y].piece = new Bishop(*players[0], *this, x, y);
                    else if (y == 7)
                        board[x][y].piece = new King(*players[0], *this, x, y);
                    else if (y == 6)
                        board[x][y].piece = new Queen(*players[0], *this, x, y);
                }
            }
        }
    }

    // Computing possible moves for all pieces on the board.
    for (int row = 0; row < 14; ++row)
    {
        for (int col = 0; col < 14; ++col)
        {
            if (board[row][col].piece != nullptr)
            {
                board[row][col].piece->list_possible_moves();
            }
        }
    }

    // Giving player turn to Player 1 upon game start
    players[0]->set_status("Playing...");

    //Displaying which Players are bots and which are not
    mw->set_bot_notif(players[1]->get_is_bot(), players[2]->get_is_bot(), players[3]->get_is_bot());
}

Game::~Game()
{
    //Game Destructor to delete all player objects
    for (int i = 0; i < 4; ++i)
    {
        delete players[i];
    }
}

Player *Game::get_player_at(int index)
{
    //Return player object at particular index.
    return players[index];
}

void Game::deactivate_player(int index, std::string status)
{
    //Function to deactivate player at selected index.
    players[index]->deactivate();
    players[index]->set_status(status);

    //Display updated status of Player in Main Window
    mw->update_status(players[0]->get_status(), players[1]->get_status(), players[2]->get_status(), players[3]->get_status());
}

void Game::print_board()
{
    //Function to print board in terminal for debugging purposes
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {
            if (!board[i][j].is_activated)
            {
                std::cout << "XX ";
            }
            else if (board[i][j].piece == nullptr)
            {
                std::cout << "   ";
            }
            else
            {
                switch (board[i][j].piece->get_type())
                {
                case Piece::Type::BISHOP:
                    std::cout << "B";
                    break;
                case Piece::Type::KING:
                    std::cout << "K";
                    break;
                case Piece::Type::KNIGHT:
                    std::cout << "N";
                    break;
                case Piece::Type::PAWN:
                    std::cout << "P";
                    break;
                case Piece::Type::QUEEN:
                    std::cout << "Q";
                    break;
                case Piece::Type::ROOK:
                    std::cout << "R";
                    break;
                default:
                    std::cout << " ";
                }
                switch (board[i][j].piece->which_player())
                {
                case 0:
                    std::cout << "1 ";
                    break;
                case 1:
                    std::cout << "2 ";
                    break;
                case 2:
                    std::cout << "3 ";
                    break;
                case 3:
                    std::cout << "4 ";
                    break;
                default:
                    std::cout << "  ";
                }
            }
        }
        std::cout << std::endl;
    }
}

Piece *Game::get_piece(int x, int y)
{
    // Return piece object pointed to by tile at [x, y] index in the Chess Board
    if (in_boundaries(x, y) && board[x][y].piece != nullptr)
    {
        return board[x][y].piece;
    }
    // Return nullptr if Tile is empty
    return nullptr;
}

int Game::get_local_player()
{
    //Return the index of the local player
    return local_player;
}

bool Game::in_boundaries(int x, int y)
{
    // Function to determine whether tile at [x,y] is within board boundaries
    if (x < 14 && x > -1 && y < 14 && y > -1 && board[x][y].is_activated == true)
    {
        return true;
    }
    return false;
}

void Game::movePiece(int initX, int initY, int destX, int destY) //This needs to be changes 21/11/21
{
    // Function to move piece from initial position to destination

    //Is initial position and destination are the same.
    if ((initX == destX) && (initY == destY))
    {
        return;
    }

    //For debugging purposes
    Piece *test = board[initX][initY].piece;
    qDebug() << test << " " << initX << initY << destX << destY;

    //Determine player index of player owning the selected piece
    int player_num = board[initX][initY].piece->which_player();

    //Logic for CAPTURING opponent pieces
    if (board[destX][destY].piece != nullptr)
    {
        players[player_num]->increase_score(board[destX][destY].piece->get_point());
        board[destX][destY].piece->set_is_captured();
        board[destX][destY].piece->setX_0();
        board[destX][destY].piece->setY_0();
    }

    //Logic for PAWN PROMOTION
    Piece *piece_to_move = board[initX][initY].piece;
    if (piece_to_move->get_type() == Piece::Type::PAWN &&
        ((destX == 7 && player_num == 2) || (destX == 6 && player_num == 0) || (destY == 7 && player_num == 1) ||
         (destY == 6 && player_num == 3)))
    {
        for (int i = 0; i < 16; ++i)
        {
            if (piece_to_move == players[player_num]->pieces[i])
            {
                delete players[player_num]->pieces[i];
                players[player_num]->pieces[i] = new Queen(*players[player_num], *this, initX, initY, i);
                board[initX][initY].piece = nullptr;
                board[destX][destY].piece = players[player_num]->pieces[i];
                board[destX][destY].piece->setCoordinates(destX, destY);
                players[player_num]->pieces[i]->set_points(1);
            }
        }
    }

    //Logic for CASTLING
    else if (piece_to_move->get_type() == Piece::Type::KING)
    {
        //Check if the move is for castling
        if (std::abs(destX - initX) == 3 || std::abs(destX - initX) == 2 || std::abs(destY - initY) == 3 || std::abs(destY - initY) == 2)
        {
            //Check if it hasn't moved
            if (!piece_to_move->get_has_moved())
            {
                int player = piece_to_move->which_player();

                // If-Else-If Block to check the rook next to the movement spot.

                // Player 1:
                if (player == 0)
                {
                    if ((destY - initY) == 2 && board[destX][destY + 1].piece != nullptr && board[destX][destY + 1].piece->get_type() == Piece::Type::ROOK && !board[destX][destY + 1].piece->get_has_moved())
                    {
                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX][destY - 1].piece = board[destX][destY + 1].piece;
                        board[destX][destY + 1].piece = nullptr;
                        board[destX][destY - 1].piece->setCoordinates(destX, destY - 1);
                        board[destX][destY - 1].piece->set_has_moved(true);
                    }
                    else if ((destY - initY) == -2 && board[destX][destY - 2].piece != nullptr && board[destX][destY - 2].piece->get_type() == Piece::Type::ROOK && !board[destX][destY - 2].piece->get_has_moved())
                    {
                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX][destY + 1].piece = board[destX][destY - 2].piece;
                        board[destX][destY - 2].piece = nullptr;
                        board[destX][destY + 1].piece->setCoordinates(destX, destY + 1);
                        board[destX][destY + 1].piece->set_has_moved(true);
                    }
                }

                // Player 2:
                else if (player == 1)
                {
                    if ((destX - initX) == -2 && board[destX - 1][destY].piece->get_type() == Piece::Type::ROOK && !board[destX - 1][destY].piece->get_has_moved())
                    {

                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX + 1][destY].piece = board[destX - 1][destY].piece;
                        board[destX - 1][destY].piece = nullptr;
                        board[destX + 1][destY].piece->setCoordinates(destX + 1, destY);
                        board[destX + 1][destY].piece->set_has_moved(true);
                    }
                    else if ((destX - initX) == 2 && board[destX + 2][destY].piece != nullptr && board[destX + 2][destY].piece->get_type() == Piece::Type::ROOK && !board[destX + 2][destY].piece->get_has_moved())
                    {
                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX - 1][destY].piece = board[destX + 2][destY].piece;
                        board[destX + 2][destY].piece = nullptr;
                        board[destX - 1][destY].piece->setCoordinates(destX - 1, destY);
                        board[destX - 1][destY].piece->set_has_moved(true);
                    }
                }

                //Player 3:
                else if (player == 2)
                {
                    if ((destY - initY) == -2 && board[destX][destY - 1].piece != nullptr && board[destX][destY - 1].piece->get_type() == Piece::Type::ROOK && !board[destX][destY - 1].piece->get_has_moved())
                    {
                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX][destY + 1].piece = board[destX][destY - 1].piece;
                        board[destX][destY - 1].piece = nullptr;
                        board[destX][destY + 1].piece->setCoordinates(destX, destY + 1);
                        board[destX][destY + 1].piece->set_has_moved(true);
                    }
                    else if ((destY - initY) == 2 && board[destX][destY + 2].piece != nullptr && board[destX][destY + 2].piece->get_type() == Piece::Type::ROOK && !board[destX][destY + 2].piece->get_has_moved())
                    {
                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX][destY - 1].piece = board[destX][destY + 2].piece;
                        board[destX][destY + 2].piece = nullptr;
                        board[destX][destY - 1].piece->setCoordinates(destX, destY - 1);
                        board[destX][destY - 1].piece->set_has_moved(true);
                    }
                }

                //Player 4:
                else if (player == 3)
                {
                    if ((destX - initX) == 2 && board[destX + 1][destY].piece->get_type() == Piece::Type::ROOK && !board[destX + 1][destY].piece->get_has_moved())
                    {
                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX - 1][destY].piece = board[destX + 1][destY].piece;
                        board[destX + 1][destY].piece = nullptr;
                        board[destX - 1][destY].piece->setCoordinates(destX - 1, destY);
                        board[destX - 1][destY].piece->set_has_moved(true);
                    }
                    else if ((destX - initX) == -2 && board[destX - 2][destY].piece != nullptr && board[destX - 2][destY].piece->get_type() == Piece::Type::ROOK && !board[destX - 2][destY].piece->get_has_moved())
                    {
                        board[destX][destY].piece = board[initX][initY].piece;
                        board[initX][initY].piece = nullptr;
                        board[destX][destY].piece->setCoordinates(destX, destY);
                        board[destX][destY].piece->set_has_moved(true);
                        board[destX + 1][destY].piece = board[destX - 2][destY].piece;
                        board[destX - 2][destY].piece = nullptr;
                        board[destX + 1][destY].piece->setCoordinates(destX + 1, destY);
                        board[destX + 1][destY].piece->set_has_moved(true);
                    }
                }
            }
        }

        //Handling ordinary move for rooks in initial position
        else
        {
            board[destX][destY].piece = board[initX][initY].piece;
            board[initX][initY].piece = nullptr;
            board[destX][destY].piece->setCoordinates(destX, destY);
            board[destX][destY].piece->set_has_moved(true);
        }
    }

    //Ordinary move method for all other pieces that don't require special moves.
    else
    {
        board[destX][destY].piece = board[initX][initY].piece;
        board[initX][initY].piece = nullptr;
        board[destX][destY].piece->setCoordinates(destX, destY);
        board[destX][destY].piece->set_has_moved(true);
    }

    // If game is online and if the player is not online nor is a bot, send the message to the server on movement
    if (is_online && !get_cur_player_pointer()->get_is_online() && !get_cur_player_pointer()->get_is_bot())
    {
        client->move(initX, initY, destX, destY);
    }
    make_turn();
}

// The function called when a bot needs to move.
void Game::bot_move_piece()
{
    int index;
    int player_num = turn_number % 4;
    std::vector<std::vector<int>> possible_move_list;

    //To get a random move out of a player's possible moves list
    do
    {
        index = get_random_number(0, 15);
        if (!players[player_num]->pieces[index]->get_is_captured() && in_boundaries(players[player_num]->pieces[index]->getX(), players[player_num]->pieces[index]->getY()) == true)
        {
            for (int i = 0; i < 14; ++i)
            {
                for (int j = 0; j < 14; ++j)
                {
                    if (players[player_num]->pieces[index]->get_possible_move_at(i, j) == 2)
                    {
                        possible_move_list.push_back(std::vector<int>());
                        possible_move_list[possible_move_list.size() - 1].push_back(i);
                        possible_move_list[possible_move_list.size() - 1].push_back(j);
                    }
                } // Check for moves, if no moves, repeat, if moves, randomly select one
            }
        }
    } while (possible_move_list.size() == 0);

    // Select a move based on the random number generator
    int move_index = get_random_number(0, possible_move_list.size() - 1);

    // Create the move and then execute it on the previous movePiece function
    int initX = players[player_num]->pieces[index]->getX();
    int initY = players[player_num]->pieces[index]->getY();
    int destX = possible_move_list[move_index][0];
    int destY = possible_move_list[move_index][1];
    std::cout << "DEBUG" << std::endl;

    std::cout << in_boundaries(initX, initY) << std::endl;
    std::cout << get_piece(initX, initY) << std::endl;
    movePiece(initX, initY, destX, destY);
    //If the game is online, publish the move to the server
    if (is_online)
    {
        client->moveBot(initX, initY, destX, destY);
    }
    return;
}


//Make a board copy for check and stalemate checks.
void Game::create_board_copy(int current_turn, int (&board_copy)[14][14])
{
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {
            board_copy[i][j] = 0;
        }
    }

    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {
            Piece *piece = board[i][j].piece;
            if (piece == nullptr || piece->get_is_captured())
                board_copy[i][j] = 0;
            //For the map, we only care about the types
            else if (piece->which_player() == current_turn)
            {
                if (piece->get_type() == Piece::Type::KING)
                {
                    board_copy[i][j] = 1;
                }
                else
                    board_copy[i][j] = 2;
            }
            else
            {
                if (piece->get_type() == Piece::Type::QUEEN)
                    board_copy[i][j] = 3;
                if (piece->get_type() == Piece::Type::ROOK)
                    board_copy[i][j] = 4;
                if (piece->get_type() == Piece::Type::BISHOP)
                    board_copy[i][j] = 5;
                if (piece->get_type() == Piece::Type::KNIGHT)
                    board_copy[i][j] = 6;
                if (piece->get_type() == Piece::Type::KING)
                    board_copy[i][j] = 7;
                if (piece->get_type() == Piece::Type::PAWN)
                {
                    board_copy[i][j] = 8 + piece->which_player();
                }
            }
        }
    }
}

void Game::make_turn()
{
    //Set previous player as waiting
    if (players[turn_number % 4]->is_in_game())
    {
        players[turn_number % 4]->set_status("Waiting...");
    }
    int current_turn;
    Player *current_player;
    //Modify the indicators until you reach to an active player
    do
    {
        ++turn_number;
        player_turn = (player_turn + 1) % 4;
        current_turn = turn_number % 4;
        current_player = players[current_turn];
    } while (!current_player->is_in_game());

    //Make a board copy and check all players if they are checked or not
    int board_copy[14][14];
    bool flag = false;
    for (int i = 0; i < 4; i++)
    {
        create_board_copy(current_turn + i, board_copy);
        if (is_checked(current_turn + i, board_copy) && players[(player_turn + i) % 4]->is_in_game())
        {
            //If they are checked, update ui elements
            players[(player_turn + i) % 4]->set_status("Checked");
            mw->update_status(players[0]->get_status(), players[1]->get_status(), players[2]->get_status(), players[3]->get_status());
        }
        else
        {
            if (i == 0)
            {
                flag = true;
            }
        }
    }
    //If the next player is not checked, set playing.
    if (flag)
    {
        players[(player_turn) % 4]->set_status("Playing...");
        mw->update_status(players[0]->get_status(), players[1]->get_status(), players[2]->get_status(), players[3]->get_status());
    }

    //Modify set_possible_moves arrays of each Piece to remove possible positions which would allow King to be checked
    int num_moves = 0;
    for (int k = 0; k < 16; ++k)
    {
        Piece *current_piece = current_player->pieces[k];
        if (!current_piece->get_is_captured())
        {
            num_moves += current_piece->list_possible_moves();
            for (int i = 0; i < 14; ++i)
            {
                for (int j = 0; j < 14; ++j)
                {
                    //Move the piece, and check if it's checked or not.
                    if (current_piece->get_possible_move_at(i, j) == 2)
                    {
                        int board_copy[14][14];
                        create_board_copy(current_turn, board_copy);

                        int tempX = current_piece->getX(), tempY = current_piece->getY();
                        board_copy[i][j] = board_copy[tempX][tempY];
                        board_copy[tempX][tempY] = 0;
                        // If it is, remove it from the board
                        if (is_checked(current_turn, board_copy))
                        {
                            current_piece->set_possible_move_at(i, j, 0);
                            --num_moves;
                        }
                    }
                }
            }
        }
    }


    create_board_copy(current_turn, board_copy);
    std::cout << "\n\n\n\n\n\n"
              << current_turn << '\n'
              << num_moves << "\n\n\n\n\n\n";
    // IF the total number of moves is equal to zero, and the current map is checked, it's a checkmate
    if (num_moves == 0)
    {
        if (is_checked(current_turn, board_copy))
        {
            std::cout << is_checked_player(current_turn, board_copy) << "\n\n\n\n";
            players[is_checked_player(current_turn, board_copy)]->increase_score(20);
            std::cout << "deactivating player" << current_player->get_index() << std::endl;
            deactivate_player(current_player->get_index(), "Checkmated");
        }
        // If it's not checked, it's a stalemate
        else
        {
            deactivate_player(current_player->get_index(), "Stalemated");
            for (int i = 0; i < 4; ++i)
            {
                if (players[i]->is_in_game())
                    players[i]->increase_score(10);
            }
        }
        //Deactivate players
        int num_active_players = 0;
        for (int i = 0; i < 4; ++i)
        {
            if (players[i]->is_in_game())
                num_active_players++;
        }
        if (num_active_players == 1)
        {
            is_ended = true;
        }

        make_turn();
        return;
    }

    for (int i = 0; i < 4; ++i)
    {
        std::cout << players[i]->get_score() << '\n';
    }
    // If there's only one player, end the game.

    if (is_ended)
    {
        end_game(false);
    }
    //update ui elements
    mw->update_status(players[0]->get_status(), players[1]->get_status(), players[2]->get_status(), players[3]->get_status());
}

void Game::end_game(bool end_to_disconnect)
{
    // Get the highest scoring player
    int highest_score = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (players[i]->get_score() > highest_score)
            highest_score = players[i]->get_score();
    }


    std::vector<int> highest_scores_arr(0);
    //If there are more than 1 person with the highest score
    for (int i = 0; i < 4; ++i)
    {
        if (players[i]->get_score() == highest_score)
            highest_scores_arr.push_back(i + 1);
    }

    highest_scores_arr.resize(4, 0);

    // Deregister all players from server
    if (is_online)
    {
        client->deregister();
    }
    //Push game over window
    GameOverWindow *gow = new GameOverWindow(end_to_disconnect, highest_scores_arr[0], highest_scores_arr[1], highest_scores_arr[2], highest_scores_arr[3]);
    gow->show();
    mw->close();
}

//Check all moves that can happen from king's point of view
bool Game::is_checked(int index, const int (&board_copy)[14][14])
{

    // Save player to be checked
    int kingX, kingY;

    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {
            if (board_copy[i][j] == 1)
            {
                kingX = i;
                kingY = j;
            }
        }
    }

    int xpos = kingX;
    int ypos = kingY;
    //Check left, right, up, down
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        else if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++xpos;
    }

    xpos = kingX;
    ypos = kingY;

    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --xpos;
    }

    xpos = kingX;
    ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++ypos;
    }

    xpos = kingX;
    ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --ypos;
    }
    //Check diagonals
    xpos = kingX;
    ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++xpos;
        ++ypos;
    }

    xpos = kingX;
    ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --xpos;
        ++ypos;
    }

    xpos = kingX;
    ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++xpos;
        --ypos;
    }

    xpos = kingX;
    ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            return true;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --xpos;
        --ypos;
    }

    for (int i = kingX - 1; i < kingX + 2; ++i)
    {
        for (int j = kingY - 1; j < kingY + 2; ++j)
        {
            if (in_boundaries(i, j))
            {
                if (board_copy[i][j] == 7)
                {
                    return true;
                }
            }
        }
    }
    //Check for knight moves
    if (index != 0)
    {
        if (in_boundaries(kingX + 1, kingY + 1) && board_copy[kingX + 1][kingY + 1] - 8 == 0)
        {
            return true;
        }
        if (in_boundaries(kingX + 1, kingY - 1) && board_copy[kingX + 1][kingY - 1] - 8 == 0)
        {
            return true;
        }
    }

    if (index != 1)
    {
        if (in_boundaries(kingX + 1, kingY - 1) && board_copy[kingX + 1][kingY - 1] - 8 == 1)
        {
            return true;
        }
        if (in_boundaries(kingX - 1, kingY - 1) && board_copy[kingX - 1][kingY - 1] - 8 == 1)
        {
            return true;
        }
    }

    if (index != 2)
    {
        if (in_boundaries(kingX - 1, kingY - 1) && board_copy[kingX - 1][kingY - 1] - 8 == 2)
        {
            return true;
        }
        if (in_boundaries(kingX - 1, kingY + 1) && board_copy[kingX - 1][kingY + 1] - 8 == 2)
        {
            return true;
        }
    }

    if (index != 3)
    {
        if (in_boundaries(kingX - 1, kingY + 1) && board_copy[kingX - 1][kingY + 1] - 8 == 3)
        {
            return true;
        }
        if (in_boundaries(kingX + 1, kingY + 1) && board_copy[kingX + 1][kingY + 1] - 8 == 3)
        {
            return true;
        }
    }

    if (in_boundaries(kingX + 2, kingY + 1) && board_copy[kingX + 2][kingY + 1] == 6)
    {
        return true;
    }
    if (in_boundaries(kingX + 2, kingY - 1) && board_copy[kingX + 2][kingY - 1] == 6)
    {
        return true;
    }
    if (in_boundaries(kingX - 2, kingY + 1) && board_copy[kingX - 2][kingY + 1] == 6)
    {
        return true;
    }
    if (in_boundaries(kingX - 2, kingY - 1) && board_copy[kingX - 2][kingY - 1] == 6)
    {
        return true;
    }

    if (in_boundaries(kingX + 1, kingY + 2) && board_copy[kingX + 1][kingY + 2] == 6)
    {
        return true;
    }
    if (in_boundaries(kingX + 1, kingY - 2) && board_copy[kingX + 1][kingY - 2] == 6)
    {
        return true;
    }
    if (in_boundaries(kingX - 1, kingY + 2) && board_copy[kingX - 1][kingY + 2] == 6)
    {
        return true;
    }
    if (in_boundaries(kingX - 1, kingY - 2) && board_copy[kingX - 1][kingY - 2] == 6)
    {
        return true;
    }

    return false;
}

//Returns the index value of the current player.
int Game::get_cur_player()
{
    return player_turn;
}

//Returns the pointer to the current player
Player *Game::get_cur_player_pointer() const
{
    return players[player_turn];
}

//Returns the index of which player should get the points for checkmating the player with the index passed.
//We want the earliest player to check the player with the index passed.
//This can be understood as the first player after the current player who has checked the current player.
int Game::is_checked_player(int index, const int (&board_copy)[14][14])
{
    int checkmater_index = 50;

    int kingX, kingY;

    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {
            if (board_copy[i][j] == 1)
            {
                kingX = i;
                kingY = j;
            }
        }
    }

    int xpos = kingX + 1;
    int ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            int this_check_index = board[xpos][ypos].piece->which_player(); // Stores the index of the player who has checked the current player

            // Adjusts the index in order to take into account the number
            // of turns after the current turn that this check occured
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;

            // Computes whether this player went before the player whose index is currently stored
            // as the earliest player to check the current player
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++xpos;
    }

    xpos = kingX - 1;
    ypos = kingY;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            int this_check_index = board[xpos][ypos].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --xpos;
    }

    xpos = kingX;
    ypos = kingY + 1;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            int this_check_index = board[xpos][ypos].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++ypos;
    }

    xpos = kingX;
    ypos = kingY - 1;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 4)
        {
            int this_check_index = board[xpos][ypos].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --ypos;
    }

    xpos = kingX + 1;
    ypos = kingY + 1;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            int this_check_index = board[xpos][ypos].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++xpos;
        ++ypos;
    }

    xpos = kingX - 1;
    ypos = kingY + 1;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            int this_check_index = board[xpos][ypos].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --xpos;
        ++ypos;
    }

    xpos = kingX + 1;
    ypos = kingY - 1;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            int this_check_index = board[xpos][ypos].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        ++xpos;
        --ypos;
    }

    xpos = kingX - 1;
    ypos = kingY - 1;
    while (in_boundaries(xpos, ypos))
    {
        if (board_copy[xpos][ypos] == 2)
            break;
        if (board_copy[xpos][ypos] == 3 || board_copy[xpos][ypos] == 5)
        {
            int this_check_index = board[xpos][ypos].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        else if (board_copy[xpos][ypos] != 0 && board_copy[xpos][ypos] != 1)
        {
            break;
        }
        --xpos;
        --ypos;
    }

    for (int i = kingX - 1; i < kingX + 2; ++i)
    {
        for (int j = kingY - 1; j < kingY + 2; ++j)
        {
            if (in_boundaries(i, j))
            {
                if (board_copy[i][j] == 7)
                {
                    int this_check_index = board[i][j].piece->which_player();
                    this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
                    checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
                }
            }
        }
    }

    if (index != 0)
    {
        if (in_boundaries(kingX + 1, kingY + 1) && board_copy[kingX + 1][kingY + 1] - 8 == 0)
        {
            int this_check_index = board[kingX + 1][kingY + 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        if (in_boundaries(kingX + 1, kingY - 1) && board_copy[kingX + 1][kingY - 1] - 8 == 0)
        {
            int this_check_index = board[kingX + 1][kingY - 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
    }

    if (index != 1)
    {
        if (in_boundaries(kingX + 1, kingY - 1) && board_copy[kingX + 1][kingY - 1] - 8 == 1)
        {
            int this_check_index = board[kingX + 1][kingY - 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        if (in_boundaries(kingX - 1, kingY - 1) && board_copy[kingX - 1][kingY - 1] - 8 == 1)
        {
            int this_check_index = board[kingX - 1][kingY - 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
    }

    if (index != 2)
    {
        if (in_boundaries(kingX - 1, kingY - 1) && board_copy[kingX - 1][kingY - 1] - 8 == 2)
        {
            int this_check_index = board[kingX - 1][kingY - 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        if (in_boundaries(kingX - 1, kingY + 1) && board_copy[kingX - 1][kingY + 1] - 8 == 2)
        {
            int this_check_index = board[kingX - 1][kingY + 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
    }

    if (index != 3)
    {
        if (in_boundaries(kingX - 1, kingY + 1) && board_copy[kingX - 1][kingY + 1] - 8 == 3)
        {
            int this_check_index = board[kingX - 1][kingY + 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
        if (in_boundaries(kingX + 1, kingY + 1) && board_copy[kingX + 1][kingY + 1] - 8 == 3)
        {
            int this_check_index = board[kingX + 1][kingY + 1].piece->which_player();
            this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
            checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
        }
    }

    if (in_boundaries(kingX + 2, kingY + 1) && board_copy[kingX + 2][kingY + 1] == 6)
    {
        int this_check_index = board[kingX + 2][kingY + 1].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }
    if (in_boundaries(kingX + 2, kingY - 1) && board_copy[kingX + 2][kingY - 1] == 6)
    {
        int this_check_index = board[kingX + 2][kingY - 1].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }
    if (in_boundaries(kingX - 2, kingY + 1) && board_copy[kingX - 2][kingY + 1] == 6)
    {
        int this_check_index = board[kingX - 2][kingY + 1].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }
    if (in_boundaries(kingX - 2, kingY - 1) && board_copy[kingX - 2][kingY - 1] == 6)
    {
        int this_check_index = board[kingX - 2][kingY - 1].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }

    if (in_boundaries(kingX + 1, kingY + 2) && board_copy[kingX + 1][kingY + 2] == 6)
    {
        int this_check_index = board[kingX + 1][kingY + 2].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }
    if (in_boundaries(kingX + 1, kingY - 2) && board_copy[kingX + 1][kingY - 2] == 6)
    {
        int this_check_index = board[kingX + 1][kingY - 2].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }
    if (in_boundaries(kingX - 1, kingY + 2) && board_copy[kingX - 1][kingY + 2] == 6)
    {
        int this_check_index = board[kingX - 1][kingY + 2].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }
    if (in_boundaries(kingX - 1, kingY - 2) && board_copy[kingX - 1][kingY - 2] == 6)
    {
        int this_check_index = board[kingX - 1][kingY - 2].piece->which_player();
        this_check_index = (this_check_index < index) ? this_check_index + 4 : this_check_index;
        checkmater_index = (checkmater_index > this_check_index) ? this_check_index : checkmater_index;
    }

    return ((checkmater_index > 3) ? checkmater_index - 4 : checkmater_index);
}

// Returns if the game is an online game or not
bool Game::get_is_online()
{
    return is_online;
}
// Returns the index of the player who's actively playing on this computer and is on networking-version of the game (used for screen rotation purposes)
int Game::get_online_player()
{
    return local_player;
}

// Returns how many active (not checkmated/disconnected) players are in a game
int Game::active_players()
{
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (players[i]->is_in_game())
        {
            count++;
        }
    }
    return count;
}
