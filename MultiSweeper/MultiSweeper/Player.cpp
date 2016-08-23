#include "Player.h"

using namespace std;

Player::Player(string username) :
	username_(username)
{
	this->special_used_ = false;
	this->games_played_ = 0;
	this->games_won_ = 0;
	this->mines_missed_ = 0;
	this->mines_revealed_ = 0;
}

Player::~Player()
{
}

string const& Player::get_username() const {
	return username_;
}

bool Player::operator==(const Player& other) const {
	return this->username_ == other.username_;
}

ostream& operator<<(ostream& os, const Player& obj) {
	os << obj.username_;
	return os;
}