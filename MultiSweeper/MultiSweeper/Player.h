#pragma once

#include <string>

class Player
{
public:
	Player(std::string username);
	~Player();

	std::string const& getUsername() const;
	bool operator==(const Player& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);
private:
	std::string username;

	bool specialUsed;
	int minesRevealed;
	int minesMissed;
	
	int gamesWon;
	int gamesPlayed; 
};

