#include "InterfaceVisual.h"
#include "SweeperException.h"

InterfaceVisual::InterfaceVisual()
{
}


InterfaceVisual::~InterfaceVisual()
{
}

void InterfaceVisual::dispatch_error(const SweeperError& error) 
{
	throw SweeperException(error);
}
