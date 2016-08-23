#include "SweeperError.h"

const SweeperError SweeperError::OK(0, "Success");

SweeperError::SweeperError(int error_code, std::string message) :
	error_code_(error_code), message_(message)
{
}

SweeperError::~SweeperError()
{
}

std::string const& SweeperError::get_message() const {
	return message_;
}

int const& SweeperError::get_error_code() const {
	return error_code_;
}