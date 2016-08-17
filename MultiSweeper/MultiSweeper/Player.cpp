#include "Player.h"

using namespace std;

Player::Player(string username) :
	username(username)
{
	this->specialUsed = false;
	this->gamesPlayed = 0;
	this->gamesWon = 0;
	this->minesMissed = 0;
	this->minesRevealed = 0;
}

Player::~Player()
{
}

string const& Player::getUsername() const {
	return username;
}

bool Player::operator==(const Player& other) const {
	return this->username == other.username;
}
