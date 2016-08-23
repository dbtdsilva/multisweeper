#pragma once

#include "SweeperError.h"

class SweeperException : public std::runtime_error
{
public:
	SweeperException(SweeperError err);
	SweeperException(const SweeperException& ex);
	~SweeperException();

	virtual const char* what() const throw();
	SweeperError const& get_sweeper_error() const;
private:
	const SweeperError err_;
};

