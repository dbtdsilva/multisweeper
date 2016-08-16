#include "Curses.h"
#include <locale.h>
#include "Console.h"

using namespace std;
typedef struct command cmd;

Curses::Curses()
{
	setlocale(LC_ALL, "");

	this->width = 120;
	this->height = 14;

	this->new_option = 0;
	this->old_option = -1;

	this->pSweeperConsole = unique_ptr<Console>(new Console(window));
	this->pEngine = unique_ptr<Engine>(new Engine(pSweeperConsole.get()));

	this->mMainuOptions = {
		{ "Instructions", [=](WINDOW * win) { state = INSTRUCTIONS; } },
		{ "Modify Board", [=](WINDOW * win) { state = BOARD_OPTIONS; } },
		{ "Add/Remove Players", [=](WINDOW * win) { state = PLAYERS_OPTIONS; } },
		{ "Start Game", [=](WINDOW * win) { state = GAME; } },
		{ "Exit", [=](WINDOW * win) { state = EXIT_REQUEST; } }
	};

	this->mBoardOptions = {
		{ "Change rows", [=](WINDOW * win) { state = MODIFY_ROWS; } },
		{ "Change lines", [=](WINDOW * win) { state = MODIFY_LINES; } },
		{ "Change number of mines", [=](WINDOW * win) { state = MODIFY_MINES; } },
		{ "Return to Main Menu", [=](WINDOW * win) { state = MAIN_MENU; } }
	};

	this->mPlayersOptions = {
		{ "Add player", [=](WINDOW * win) { state = PLAYER_ADD; } },
		{ "Remove player", [=](WINDOW * win) { state = PLAYER_REMOVE; } },
		{ "Return to Main Menu", [=](WINDOW * win) { state = MAIN_MENU; } }
	};
	this->state = MAIN_MENU;
	
	init();
}

Curses::~Curses()
{
}

void Curses::loop() {
	do {
		displayCurses();
		int key = getch();
		processKey(key);		
	} while (state != EXIT_REQUEST);

	delwin(window);
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

	window = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
	if (window == NULL)
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
		wbkgd(window, COLOR_PAIR(1));
	} else {
		wbkgd(window, A_REVERSE);
	}

	// Removed cursor visibility from terminal
	curs_set(false);
	// Removed the output of the key input from the user
	noecho();
	// Changed to keypad to accept KEY_HOME, KEY_UP, etc
	keypad(stdscr, true);
	// Characters typed are immediately passed through to the user program
	raw();
	erase();
}

void Curses::displayCurses() {
	switch (state) {
	case MAIN_MENU:
		displayMenu(mMainuOptions);
		break;
	case BOARD_OPTIONS:
		displayMenu(mBoardOptions);
		break;
	case INSTRUCTIONS:
		break;
	case PLAYERS_OPTIONS:
		displayMenu(mPlayersOptions);
		break;
	case PLAYER_ADD:
		break;
	case PLAYER_REMOVE:
		break;
	case MODIFY_LINES:
		break;
	case MODIFY_ROWS:
		break;
	case MODIFY_MINES:
		break;
	case GAME:
		break;
	}
}

void Curses::processKey(int key) {
	switch (state) {
	case MAIN_MENU:
		processMenuKey(key, mMainuOptions);
		break;
	case BOARD_OPTIONS:
		processMenuKey(key, mBoardOptions);
		break;
	case INSTRUCTIONS:
		state = MAIN_MENU;	// Any key to continue
		break;
	case PLAYERS_OPTIONS:
		processMenuKey(key, mPlayersOptions);
		break;
	case PLAYER_ADD:
		break;
	case PLAYER_REMOVE:
		break;
	case MODIFY_LINES:
		break;
	case MODIFY_ROWS:
		break;
	case MODIFY_MINES:
		break;
	case GAME:
		break;
	}
}

void Curses::displayMenu(vector<cmd> options)
{
	int lmarg = (COLS - height) / 2;
	int tmarg = (LINES - ((int)options.size() + 2)) / 2;
	int menuMargin = 7;

	if (old_option == -1)
	{
		int i;

		attrset(A_BOLD);
		string headerMessage = "MultiSweeper Console";
		mvaddstr(2, (COLS - (int)headerMessage.size()) / 2, headerMessage.c_str());
		attrset(A_NORMAL);

		for (i = 0; i < options.size(); i++)
			mvaddstr(menuMargin + i, (COLS - (int)options[i].text.size()) / 2, options[i].text.c_str());
	}
	else
		mvaddstr(menuMargin + old_option, (COLS - (int)options[old_option].text.size()) / 2, options[old_option].text.c_str());

	attrset(A_REVERSE);
	mvaddstr(menuMargin + new_option, (COLS - (int)options[new_option].text.size()) / 2, options[new_option].text.c_str());
	attrset(A_NORMAL);


	string bottomMessage = "Use UP and DOWN Arrows to move and ENTER to select";
	mvaddstr(LINES - 3, (COLS - (int)bottomMessage.length()) / 2, bottomMessage.c_str());
	refresh();
}

void Curses::processMenuKey(int key, vector<cmd> options)
{
	switch (key)
	{
	case 10:
	case 13:
	case KEY_ENTER:
		old_option = -1;
		erase();
		options[new_option].function(window);
		new_option = 0;
		break;

	case KEY_PPAGE:
	case KEY_HOME:
		old_option = new_option;
		new_option = 0;
		break;

	case KEY_NPAGE:
	case KEY_END:
		old_option = new_option;
		new_option = (int)options.size() - 1;
		break;

	case KEY_UP:
		old_option = new_option;
		new_option = (new_option == 0) ?
			new_option : new_option - 1;
		break;

	case KEY_DOWN:
		old_option = new_option;
		new_option = (new_option == options.size() - 1) ?
			new_option : new_option + 1;
		break;
	}
}
