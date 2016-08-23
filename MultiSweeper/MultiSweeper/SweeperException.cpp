#include "SweeperException.h"

#include <sstream>

using namespace std;

SweeperException::SweeperException(SweeperError err) :
	err(err), runtime_error(err.get_message())
{
}

SweeperException::~SweeperException()
{
}

const char* SweeperException::what() const throw() {
	stringstream ss;
	ss << "Error " << err.get_error_code() << ":" << err.get_message();
	return ss.str().c_str();
}