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
	
	std::string const& get_message() const;
	int const& get_error_code() const;

private:
	SweeperError(int error_code, std::string message);

	int error_code_;
	std::string message_;
};

