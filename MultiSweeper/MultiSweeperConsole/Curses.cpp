#include "Curses.h"
#include <locale.h>
#include <iostream>
#include <sstream>
#include "Console.h"

using namespace std;
typedef struct command cmd;

Curses::Curses()
{
	setlocale(LC_ALL, "");

	this->width = 120;
	this->height = 30;

	this->new_option = 0;
	this->old_option = -1;

	this->mCols = 20;
	this->mRows = 10;
	this->mMines = 10;

	this->pSweeperConsole = unique_ptr<Console>(new Console(window));
	this->pEngine = unique_ptr<Engine>(new Engine(pSweeperConsole.get(), mRows, mCols, mMines));

	this->mMainuOptions = {
		{ "Instructions", [=](WINDOW * win) { state = INSTRUCTIONS; } },
		{ "Modify Board", [=](WINDOW * win) { state = BOARD_OPTIONS; } },
		{ "Add/Remove Players", [=](WINDOW * win) { state = PLAYERS_OPTIONS; } },
		{ "Start Game", [=](WINDOW * win) { state = GAME; } },
		{ "Exit", [=](WINDOW * win) { state = EXIT_REQUEST; } }
	};

	this->mBoardOptions = {
		{ "Change rows", [=](WINDOW * win) { state = MODIFY_ROWS; } },
		{ "Change lines", [=](WINDOW * win) { state = MODIFY_COLS; } },
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

	window = newwin(height, width, 0, 0);
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
		displayInstructions();
		break;
	case PLAYERS_OPTIONS:
		displayMenu(mPlayersOptions);
		break;
	case PLAYER_ADD:
		break;
	case PLAYER_REMOVE:
		break;
	case MODIFY_COLS:
		modifyCols();
		break;
	case MODIFY_ROWS:
		modifyRows();
		break;
	case MODIFY_MINES:
		modifyMines();
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
		erase();
		break;
	case PLAYERS_OPTIONS:
		processMenuKey(key, mPlayersOptions);
		break;
	case PLAYER_ADD:
		break;
	case PLAYER_REMOVE:
		break;
	case MODIFY_COLS:
	case MODIFY_ROWS:
	case MODIFY_MINES:
		state = BOARD_OPTIONS;	// Any key to continue
		erase();
		break;
	case GAME:
		break;
	}
}

void Curses::displayInstructions() {
	attrset(A_BOLD);
	mvaddstrCentered(1, "INSTRUCTIONS");
	attrset(A_NORMAL);

	mvaddstrCentered(3, "The objective of the game consists in DISCOVERING the MAXIMUM ");
	mvaddstrCentered(4, "number of MINES as possible. ");
	mvaddstrCentered(5, "There will be a board 2D with mines hidden on it. Until players");
	mvaddstrCentered(6, "find all the mines on the board, the game will not finish.");
	mvaddstrCentered(7, "On your turn, you might select a position to reveal.");

	attrset(A_BOLD);
	mvaddstrCentered(9, "Every square has a meaning:");
	attrset(A_NORMAL);
	// Left aligned and centered according to the biggest bullet item
	mvaddstr(10, (COLS - 62) / 2, "* Number on the square is the number of adjacent mines;");
	mvaddstr(11, (COLS - 62) / 2, "\tE.g. \"4\" means that there is 4 mines adjacent to that square");
	mvaddstr(12, (COLS - 62) / 2, "* \"X\" is a revealed mine;");
	mvaddstr(13, (COLS - 62) / 2, "* \"O\" appears when there is no adjacent mines to this square;");
	mvaddstr(14, (COLS - 62) / 2, "* If the square is empty, it has not been revealed yet;");

	attrset(A_BOLD);
	mvaddstrCentered(16, "Press any key to continue");
	attrset(A_NORMAL);
}

void Curses::displayBoardStatus(int row) {
	stringstream ss;
	ss << "Board has " << mRows << " rows and " << mCols << " columns";
	ss << " with " << mMines << " mines";
	mvaddstrCentered(row, ss.str());
}
void Curses::modifyRows() {
	int nRows;
	displayBoardStatus(1);
	mvscanwRobust("Enter the total number of ROWS", 3, &nRows);
	this->mRows = nRows;
	pEngine->modifyBoard(mRows, mCols);
	displayBoardStatus(6);
}

void Curses::modifyCols() {
	int nCols;
	displayBoardStatus(1);
	mvscanwRobust("Enter the total number of COLUMNS", 3, &nCols);
	this->mCols = nCols;
	pEngine->modifyBoard(mRows, mCols);
	displayBoardStatus(6);
}

void Curses::modifyMines() {
	int nMines;
	displayBoardStatus(1);
	mvscanwRobust("Enter the total number of MINES", 3, &nMines);
	this->mMines = nMines;
	pEngine->modifyNumberMines(nMines);
	displayBoardStatus(6);
}

void Curses::mvaddstrCentered(int row, string str) {
	mvaddstr(row, (COLS - (int)str.size()) / 2, str.c_str());
}

void Curses::displayMenu(vector<cmd> options)
{
	int menuMargin = 7;

	if (old_option == -1) {
		int i;

		attrset(A_BOLD);
		mvaddstrCentered(2, "MultiSweeper Console");
		attrset(A_NORMAL);

		for (i = 0; i < options.size(); i++)
			mvaddstrCentered(menuMargin + i, options[i].text);
	} else {
		mvaddstrCentered(menuMargin + old_option, options[old_option].text);
	}

	attrset(A_REVERSE);
	mvaddstrCentered(menuMargin + new_option, options[new_option].text);
	attrset(A_NORMAL);

	mvaddstrCentered(LINES - 3, "Use UP and DOWN Arrows to move and ENTER to select");
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

template<typename T>
void Curses::mvscanwRobust(string introText, int rowStart, T * returnValue)
{
	attrset(A_BOLD);
	mvaddstrCentered(rowStart, introText);
	attrset(A_NORMAL);
	refresh();
	echo();
	curs_set(true);
	if (is_same<T, int>::value) {
		mvscanw(rowStart + 1, (COLS - 2) / 2, "%d", returnValue);
	}
	else if (is_same<T, string>::value) {
		mvscanw(rowStart + 1, (COLS - 10) / 2, "%s", returnValue);
	}
	else {
		throw runtime_error("Invalid type received into the function readValue");
	}
	noecho();
	curs_set(false);
	attrset(A_BOLD);
	mvaddstrCentered(rowStart + 4, "Press any key to continue");
	attrset(A_NORMAL);
}