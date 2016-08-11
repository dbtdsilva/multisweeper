#include <iostream>

#include "SweeperConsole.h"
#include "Engine.h"

using namespace std;

/*
* This is a test program for PDCurses. Originally by
* John Burnell <johnb@kea.am.dsir.govt.nz>
*
*  wrs(5/28/93) -- modified to be consistent (perform identically)
*                  with either PDCurses or under Unix System V, R4
*
*  $Id: testcurs.c,v 1.85 2008/07/14 12:35:23 wmcbrine Exp $
*/

#ifndef _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>

#include <locale.h>

#ifdef A_COLOR
# define HAVE_COLOR 1
#else
# define HAVE_COLOR 0
#endif

void inputTest(WINDOW *);
void introTest(WINDOW *);
int initTest(WINDOW **, int, char **);
void outputTest(WINDOW *);
void padTest(WINDOW *);
void acsTest(WINDOW *);

void display_menu(int, int);

struct commands
{
	const char *text;
	void(*function)(WINDOW *);
};

typedef struct commands COMMAND;

#define MAX_OPTIONS 4

COMMAND command[MAX_OPTIONS] =
{
	{ "Instructions", introTest },
	{ "Modify Board", introTest },
	{ "Add/Remove Players", introTest },
	{ "Start Game", introTest }
};

int width, height;

int main(int argc, char *argv[])
{
	unique_ptr<SweeperConsole> pSweeperConsole = unique_ptr<SweeperConsole>(new SweeperConsole());
	unique_ptr<Engine> pEngine = unique_ptr<Engine>(new Engine(pSweeperConsole.get()));
	WINDOW *win;
	int key, old_option = -1, new_option = 0;
	bool quit = FALSE;

	setlocale(LC_ALL, "");

	if (initTest(&win, argc, argv))
		return 1;

#ifdef A_COLOR
	if (has_colors())
	{
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		wbkgd(win, COLOR_PAIR(1));
	}
	else
#endif
		wbkgd(win, A_REVERSE);

	erase();
	display_menu(old_option, new_option);

	while (1)
	{
		noecho();
		keypad(stdscr, TRUE);
		raw();

		key = getch();

		switch (key)
		{
		case 10:
		case 13:
		case KEY_ENTER:
			old_option = -1;
			erase();
			refresh();
			(*command[new_option].function)(win);
			erase();
			display_menu(old_option, new_option);
			break;

		case KEY_PPAGE:
		case KEY_HOME:
			old_option = new_option;
			new_option = 0;
			display_menu(old_option, new_option);
			break;

		case KEY_NPAGE:
		case KEY_END:
			old_option = new_option;
			new_option = MAX_OPTIONS - 1;
			display_menu(old_option, new_option);
			break;

		case KEY_UP:
			old_option = new_option;
			new_option = (new_option == 0) ?
				new_option : new_option - 1;
			display_menu(old_option, new_option);
			break;

		case KEY_DOWN:
			old_option = new_option;
			new_option = (new_option == MAX_OPTIONS - 1) ?
				new_option : new_option + 1;
			display_menu(old_option, new_option);
			break;
		case 'Q':
		case 'q':
			quit = TRUE;
		}

		if (quit == TRUE)
			break;
	}

	delwin(win);
	endwin();

	return 0;
}

void Continue(WINDOW *win)
{
	mvwaddstr(win, 10, 1, " Press any key to continue");
	wrefresh(win);
	raw();
	wgetch(win);
}

int initTest(WINDOW **win, int argc, char *argv[])
{
#ifdef XCURSES
	Xinitscr(argc, argv);
#else
	initscr();
#endif

#ifdef A_COLOR
	if (has_colors())
		start_color();
#endif
	/* Create a drawing window */

	width = 60;
	height = 13;

	*win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

	if (*win == NULL)
	{
		endwin();
		return 1;
	}

	return 0;
}

void introTest(WINDOW *win)
{
	werase(win);
	wmove(win, height / 2 - 5, width / 2);
	wvline(win, ACS_VLINE, 10);
	wmove(win, height / 2, width / 2 - 10);
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

void display_menu(int old_option, int new_option)
{
	int lmarg = (COLS - 14) / 2,
		tmarg = (LINES - (MAX_OPTIONS + 2)) / 2;

	if (old_option == -1)
	{
		int i;

		attrset(A_BOLD);
		mvaddstr(tmarg - 3, lmarg - 5, "MultiSweeper Console");
		attrset(A_NORMAL);

		for (i = 0; i < MAX_OPTIONS; i++)
			mvaddstr(tmarg + i, lmarg, command[i].text);
	}
	else
		mvaddstr(tmarg + old_option, lmarg, command[old_option].text);

	attrset(A_REVERSE);
	mvaddstr(tmarg + new_option, lmarg, command[new_option].text);
	attrset(A_NORMAL);

	mvaddstr(tmarg + MAX_OPTIONS + 2, lmarg - 23,
		"Use Up and Down Arrows to select - Enter to run - Q to quit");
	refresh();
}
