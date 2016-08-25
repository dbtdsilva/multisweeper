#pragma once

#include <string>

class Player
{
public:
	Player(std::string username);

	void update_game_stats(bool won);
	void increase_mines_revealed();
	void increase_mines_missed();
	void surrender();
	
	std::string const& get_username() const;
	int const& get_mines_revealed() const;
	int const& get_games_won() const;
	int const& get_games_played() const;
	bool const& has_surrendered() const;

	bool operator==(const Player& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);
private:
	std::string username_;
	bool special_used_;
	int mines_revealed_;
	int mines_missed_;	
	int games_won_;
	int games_tied_;
	int games_played_; 
	bool surrender_;
};

