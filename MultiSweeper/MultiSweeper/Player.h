#pragma once

#include <string>

class Player
{
public:
	Player();
	~Player();

private:
	std::string username;

	bool specialUsed;
	int minesRevealed;
	int minesMissed;
	
	int gamesWon;
	int gamesPlayed; 
};

