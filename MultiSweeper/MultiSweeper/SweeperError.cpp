#include "SweeperError.h"

SweeperError::SweeperError(int error_code, std::string message) :
	error_code_(error_code), message_(message)
{
}

SweeperError::SweeperError(const SweeperError& error) :
	SweeperError(error.error_code_, error.message_)
{
}

std::string const& SweeperError::get_message() const 
{
	return message_;
}

int const& SweeperError::get_error_code() const 
{
	return error_code_;
}

const SweeperError SweeperError::SUCCESS(0, "Success");
const SweeperError SweeperError::NO_PLAYERS(1, "There is no players registered");
const SweeperError SweeperError::GAME_ALREADY_STARTED(2, "Game has already started");
const SweeperError SweeperError::GAME_NOT_RUNNING(3, "Game is not running");
const SweeperError SweeperError::POSITION_ALREADY_REVEALED(4, "Position was already revealed");
const SweeperError SweeperError::MINES_EXCEED(5, "There is more mines than board fields");