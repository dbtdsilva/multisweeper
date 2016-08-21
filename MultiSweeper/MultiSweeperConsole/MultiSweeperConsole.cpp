#include <iostream>

#include "Curses.h"
#include "Engine.h"

using namespace std;

int main(int argc, char *argv[])
{
	unique_ptr<Curses> pSweeperCurses = make_unique<Curses>();
	pSweeperCurses->loop();

	return 0;
}
