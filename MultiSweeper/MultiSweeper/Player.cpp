#include "Player.h"

using namespace std;

Player::Player(string username)
{
	this->specialUsed = false;
	this->gamesPlayed = 0;
	this->gamesWon = 0;
	this->minesMissed = 0;
	this->username = username;
	this->minesRevealed = 0;
}


Player::~Player()
{
}

string Player::getUsername() {
	return username;
}
