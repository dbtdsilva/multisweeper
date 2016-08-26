/*! \brief SweeperError class helps to represent a game error.
*
*  This class stores a bunch of static const decorators that represent
*  every possible error in the game engine. Each error has a message
*  and an error code associated.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#pragma once

#include <string>

using namespace std;

class SweeperError
{
public:
	static const SweeperError SUCCESS;
	static const SweeperError NO_PLAYERS;
	static const SweeperError GAME_ALREADY_STARTED;
	static const SweeperError GAME_NOT_RUNNING;
	static const SweeperError POSITION_ALREADY_REVEALED;
	static const SweeperError MINES_EXCEED;
	static const SweeperError PLAYER_ALREADY_EXISTS;
	static const SweeperError PLAYER_USERNAME_INVALID;
	static const SweeperError PLAYER_NOT_EXISTS;
	static const SweeperError MAX_PLAYERS_REACHED;
	static const SweeperError SPECIAL_BOMB_ALREADY_USED;
	static const SweeperError PLAYER_ALREADY_SURRENDERED;
	
	std::string const& get_message() const;
	int const& get_error_code() const;

private:
	SweeperError(int error_code, std::string message);

	int error_code_;
	std::string message_;
};

