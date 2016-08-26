/*! \brief Player class which represents a Multisweeper user which stats associated.
*
*  Player class which represents a Multisweeper user which stats associated (such as,
*  games that player has won/played) and game variables.
*  Any errors will be reported using SweeperException.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

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
	void use_special();

	std::string const& get_username() const;
	int const& get_mines_revealed() const;
	int const& get_games_won() const;
	int const& get_games_played() const;
	bool const& has_surrendered() const;
	bool const& has_special() const;

	bool operator==(const Player& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);
private:
	std::string username_;
	bool special_used_;
	int mines_revealed_;
	int mines_missed_;	
	int games_won_;
	int games_played_; 
	bool surrender_;
};

