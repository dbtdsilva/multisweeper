#include "InterfaceVisual.h"
#include "SweeperException.h"

InterfaceVisual::InterfaceVisual()
{
}


InterfaceVisual::~InterfaceVisual()
{
}

void InterfaceVisual::errorOccured(const SweeperError& error) {
	throw SweeperException(error);
}
