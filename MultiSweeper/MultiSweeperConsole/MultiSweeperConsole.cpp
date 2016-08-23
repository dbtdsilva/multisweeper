#include <iostream>

#include "Curses.h"
#include "Engine.h"

using namespace std;

int main(int argc, char *argv[])
{
	unique_ptr<Curses> sweeper_curses = make_unique<Curses>();
	sweeper_curses->loop();

	return 0;
}
