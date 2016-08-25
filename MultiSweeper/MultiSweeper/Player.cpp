#include "Player.h"
#include <iostream>
using namespace std;

Player::Player(string username) :
	username_(username), special_used_(false), games_played_(0),
	mines_missed_(0), mines_revealed_(0), games_tied_(0),
	games_won_(0), surrender_(false)
{
}

void Player::update_game_stats(bool won) {
	games_played_++;
	if (won == true) {
		games_won_++;
	}
	mines_revealed_ = 0;
	mines_missed_ = 0;
	surrender_ = false;
}

void Player::increase_mines_revealed() 
{
	mines_revealed_++;
}

void Player::increase_mines_missed() 
{
	mines_missed_++;
}

void Player::surrender()
{
	surrender_ = true;
}

string const& Player::get_username() const 
{
	return username_;
}

int const& Player::get_mines_revealed() const
{
	return mines_revealed_;
}

int const& Player::get_games_won() const
{
	return games_won_;
}

int const& Player::get_games_played() const
{
	return games_played_;
}

bool const& Player::has_surrendered() const
{
	return surrender_;
}

bool Player::operator==(const Player& other) const 
{
	return this->username_ == other.username_;
}

ostream& operator<<(ostream& os, const Player& obj) 
{
	os << obj.username_;
	return os;
}