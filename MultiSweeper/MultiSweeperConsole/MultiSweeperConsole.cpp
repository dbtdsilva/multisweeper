/*! \brief MultiSweeperConsole.cpp is the main file which will execute the 
*          console game.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#include <iostream>

#include "SweeperCurses.h"

using namespace std;

int main(int argc, char *argv[])
{
	unique_ptr<SweeperCurses> sweeper_curses = make_unique<SweeperCurses>();
	sweeper_curses->loop();
	
	return 0;
}
