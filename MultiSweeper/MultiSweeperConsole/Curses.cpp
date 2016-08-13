#include "Curses.h"

#include <locale.h>

using namespace std;
typedef struct command cmd;

Curses::Curses()
{
	setlocale(LC_ALL, "");

	this->width = 120;
	this->height = 14;
}

Curses::~Curses()
{
}

void Curses::loop() {
	init();

	vector<cmd> options = {
		{ "Instructions", nullptr },
		{ "Modify Board", nullptr },
		{ "Add/Remove Players", nullptr },
		{ "Start Game", nullptr }
	};
	paintMenu(options, old_option, new_option);

	while (true)
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
			(*options[new_option].function)(win);
			erase();
			paintMenu(options, old_option, new_option);
			break;

		case KEY_PPAGE:
		case KEY_HOME:
			old_option = new_option;
			new_option = 0;
			paintMenu(options, old_option, new_option);
			break;

		case KEY_NPAGE:
		case KEY_END:
			old_option = new_option;
			new_option = (int)options.size() - 1;
			paintMenu(options, old_option, new_option);
			break;

		case KEY_UP:
			old_option = new_option;
			new_option = (new_option == 0) ?
				new_option : new_option - 1;
			paintMenu(options, old_option, new_option);
			break;

		case KEY_DOWN:
			old_option = new_option;
			new_option = (new_option == options.size() - 1) ?
				new_option : new_option + 1;
			paintMenu(options, old_option, new_option);
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
}

void Curses::init()
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

	win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
	if (win == NULL)
	{
		endwin();
		throw runtime_error("Failed to create window using PDCurses");
	}
	bool has_color_available = false;
#ifdef A_COLOR
	has_color_available = true;
#endif
	if (has_colors() && has_color_available)
	{
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		wbkgd(win, COLOR_PAIR(1));
	} else {
		wbkgd(win, A_REVERSE);
	}

	erase();
}

void Curses::paintMenu(vector<cmd> options, int old_option, int new_option)
{
	vector<cmd> mainMenu = {
		{ "Instructions", nullptr },
		{ "Modify Board", nullptr },
		{ "Add/Remove Players", nullptr },
		{ "Start Game", nullptr }
	};
	int lmarg = (COLS - height) / 2,
		tmarg = (LINES - ((int)options.size() + 2)) / 2;

	if (old_option == -1)
	{
		int i;

		attrset(A_BOLD);
		mvaddstr(tmarg - 3, lmarg - 5, "MultiSweeper Console");
		attrset(A_NORMAL);

		for (i = 0; i < options.size(); i++)
			mvaddstr(tmarg + i, lmarg, options[i].text);
	}
	else
		mvaddstr(tmarg + old_option, lmarg, options[old_option].text);

	attrset(A_REVERSE);
	mvaddstr(tmarg + new_option, lmarg, options[new_option].text);
	attrset(A_NORMAL);

	mvaddstr(tmarg + (int)options.size() + 2, lmarg - 23, "Use Up and Down Arrows to select - Enter to run - Q to quit");
	refresh();
}

void Curses::displayMenu(std::vector<cmd> vec)
{
}
