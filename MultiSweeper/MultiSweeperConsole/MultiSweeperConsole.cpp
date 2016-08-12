#include <iostream>

#include "SweeperConsole.h"
#include "SweeperCurses.h"
#include "Engine.h"

using namespace std;

#ifndef _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>



#ifdef A_COLOR
# define HAVE_COLOR 1
#else
# define HAVE_COLOR 0
#endif

void introTest(WINDOW *);


int main(int argc, char *argv[])
{
	unique_ptr<SweeperConsole> pSweeperConsole = unique_ptr<SweeperConsole>(new SweeperConsole());
	unique_ptr<Engine> pEngine = unique_ptr<Engine>(new Engine(pSweeperConsole.get()));

	unique_ptr<SweeperCurses> pSweeperCurses = unique_ptr<SweeperCurses>(new SweeperCurses());
	pSweeperCurses->loop();

	return 0;
}

void Continue(WINDOW *win)
{
	mvwaddstr(win, 10, 1, " Press any key to continue");
	wrefresh(win);
	raw();
	wgetch(win);
}



void introTest(WINDOW *win)
{
	werase(win);
	wmove(win, 32 / 2 - 5, 168 / 2);
	wvline(win, ACS_VLINE, 10);
	wmove(win, 32 / 2, 168 / 2 - 10);
	whline(win, ACS_HLINE, 20);
	Continue(win);

	beep();
	werase(win);

	box(win, ACS_VLINE, ACS_HLINE);
	wrefresh(win);

	cbreak();
	mvwaddstr(win, 1, 1,
		"You should have a rectangle in the middle of the screen");
	mvwaddstr(win, 2, 1, "You should have heard a beep");
	Continue(win);

	flash();
	mvwaddstr(win, 3, 1, "You should have seen a flash");
	Continue(win);
}


