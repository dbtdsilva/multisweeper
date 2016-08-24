#include "Player.h"

using namespace std;

Player::Player(string username) :
	username_(username), special_used_(false), games_played_(0),
	mines_missed_(0), mines_revealed_(0)
{
}

Player::~Player()
{
}

void Player::update_game_stats(bool won) {
	games_played_++;
	if (won == true) {
		games_won_++;
	}
	mines_revealed_ = 0;
	mines_missed_ = 0;
}

void Player::increase_mines_revealed() 
{
	mines_revealed_++;
}

string const& Player::get_username() const 
{
	return username_;
}

int const& Player::get_mines_reaveled() const
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

bool Player::operator==(const Player& other) const 
{
	return this->username_ == other.username_;
}

ostream& operator<<(ostream& os, const Player& obj) 
{
	os << obj.username_;
	return os;
}