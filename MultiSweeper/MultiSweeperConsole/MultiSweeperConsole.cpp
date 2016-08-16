#include <iostream>

#include "Console.h"
#include "Curses.h"
#include "Engine.h"

using namespace std;

int main(int argc, char *argv[])
{
	unique_ptr<Curses> pSweeperCurses = unique_ptr<Curses>(new Curses());
	pSweeperCurses->loop();

	return 0;
}
