#pragma once

#include <string>

class Player
{
public:
	Player(std::string username);
	~Player();

private:
	std::string username;

	bool specialUsed;
	int minesRevealed;
	int minesMissed;
	
	int gamesWon;
	int gamesPlayed; 
};

