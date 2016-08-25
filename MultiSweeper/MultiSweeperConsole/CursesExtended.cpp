#include "CursesExtended.h"

using namespace std;

CursesExtended::CursesExtended() :
	window_(nullptr), width_(10), height_(30),
	pair_colors_({
		{COLOR_WHITE, COLOR_BLACK},
		{COLOR_YELLOW, COLOR_BLACK},
		{COLOR_CYAN, COLOR_BLACK},
		{COLOR_RED, COLOR_BLACK},
		{COLOR_GREEN, COLOR_BLACK},
		{COLOR_MAGENTA, COLOR_BLACK}
	}), color_schema_index_(2)
{
	setlocale(LC_ALL, "");
	init();
}


CursesExtended::~CursesExtended()
{
}

void CursesExtended::init()
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

	window_ = newwin(height_, width_, 0, 0);
	if (window_ == NULL)
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
		for (int i = 1; i <= pair_colors_.size(); i++) 
		{
			init_pair(i, get<0>(pair_colors_[i - 1]), get<1>(pair_colors_[i - 1]));
		}
		wbkgd(window_, COLOR_PAIR(1));
	}
	else {
		wbkgd(window_, A_REVERSE);
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

void CursesExtended::mvaddstr_centered(int row, string message)
{
	mvaddstr(row, (COLS - (int)message.size()) / 2, message.c_str());
}

void CursesExtended::display_error(int row, string message)
{
	attrset(A_BOLD);
	attron(COLOR_PAIR(4));
	mvaddstr_centered(row, message);
	attroff(COLOR_PAIR(4));
	attrset(A_NORMAL);
	refresh();
}

void CursesExtended::clear_specific(int row, int size)
{
	string empty = "";
	for (int i = 0; i < size; i++)
		empty.append(" ");
	mvaddstr_centered(row, empty);
}

void CursesExtended::change_color_schema()
{
	color_schema_index_ = ++color_schema_index_ % pair_colors_.size();
}

const int& CursesExtended::get_color_schema_index()
{
	return color_schema_index_;
}

const int& CursesExtended::get_height()
{
	return height_;
}

const int& CursesExtended::get_width()
{
	return width_;
}
