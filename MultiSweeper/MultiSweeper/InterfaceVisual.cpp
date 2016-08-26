/*! \brief InterfaceVisual class represents the output of a Multisweeper game.
*
*  This class is an abstract class that needs to be inherited by an
*  interface which will be responsible to represent visually the game engine.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#include "InterfaceVisual.h"
#include "SweeperException.h"

InterfaceVisual::InterfaceVisual()
{
}

void InterfaceVisual::dispatch_error(const SweeperError& error) 
{
	throw SweeperException(error);
}
