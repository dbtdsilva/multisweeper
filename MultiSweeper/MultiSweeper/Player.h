#pragma once

#include <string>

class Player
{
public:
	Player(std::string username);
	~Player();

	std::string const& get_username() const;
	bool operator==(const Player& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);
private:
	std::string username_;
	bool special_used_;
	int mines_revealed_;
	int mines_missed_;	
	int games_won_;
	int games_played_; 
};

