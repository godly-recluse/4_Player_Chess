
#include "Player.h"
#include "Game.h"

Player::Player(Game &game) : is_active{true}, game{game}, is_bot{false} {} //Constructor to create player with its attributes, that have default values.

Player::~Player()
{
    //Player destructor also handles the deletion of all its pieces.
    for (int i = 0; i < 16; ++i)
    {
        delete pieces[i];
    }
}

void Player::set_index(int index)
{
    //Mutator Method to set player index/number.
    indexNum = index;
}

int Player::get_index()
{
    //Accessor Method to get player index/number.
    return indexNum;
}

bool Player::is_in_game()
{
    //Returns whether Player is still in game.
    return is_active;
}

void Player::increase_score(int score)
{
    //Mutator Method to increase player score.
    this->score += score;
}

int Player::get_score()
{
    //Accessor Method to get player's score.
    return score;
}

void Player::deactivate()
{
    //Function to deactivate player upon disonnection, checkmate or stalemate
    std::cout << "DEACTIVATING" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        pieces[i]->set_points(0);
    }
    is_active = false;
}

bool Player::get_is_bot() const
{
    //Accessor Method to know whether player is a bot.
    return is_bot;
}

void Player::set_is_bot()
{
    //Mutator Method used to set player as a bot.
    is_bot = true;
}

bool Player::get_is_online() const
{
    //Accessor Method to know whether player is online
    return is_online;
}

void Player::set_is_online()
{
    //Mutator Method to set player type to online.
    is_online = true;
}

std::string Player::get_status()
{
    //Accessor Method to access player's current status
    return status;
}

void Player::set_status(std::string status)
{
    //Mutator Method to set player status
    this->status = status;
}
