/*! \brief SweeperException class is a complement to SweeperError allowing
*          to throw exceptions with SweeperError objects.
*
*  SweeperException complements SweeperError inheriting the runtime_error
*  exception which already supports a message.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#include "SweeperException.h"

#include <sstream>

using namespace std;

SweeperException::SweeperException(SweeperError err) :
	err_(err), runtime_error(err.get_message())
{
}

const char* SweeperException::what() const throw() 
{
	stringstream ss;
	ss << "Error " << err_.get_error_code() << ":" << err_.get_message();
	return ss.str().c_str();
}

SweeperError const& SweeperException::get_sweeper_error() const 
{
	return err_;
}