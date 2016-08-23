#pragma once

#include <string>

class SweeperError
{
public:
	static const SweeperError OK;
	
	std::string const& get_message() const;
	int const& get_error_code() const;
private:
	SweeperError(int error_code, std::string message);
	~SweeperError();

	int error_code_;
	std::string message_;
};

