/*! \brief SweeperException class is a complement to SweeperError allowing
*          to throw exceptions with SweeperError objects.
*
*  SweeperException complements SweeperError inheriting the runtime_error
*  exception which already supports a message.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#pragma once

#include "SweeperError.h"

class SweeperException : public std::runtime_error
{
public:
	SweeperException(SweeperError err);

	virtual const char* what() const throw();
	SweeperError const& get_sweeper_error() const;
private:
	const SweeperError err_;
};

