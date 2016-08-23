#pragma once

#include "SweeperError.h"

class SweeperException : public std::runtime_error
{
public:
	SweeperException(SweeperError err);
	~SweeperException();
	virtual const char* what() const throw();
private:
	const SweeperError& err;
};

