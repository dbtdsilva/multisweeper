#include "CursesMenuExtended.h"

using namespace std;

CursesMenuExtended::CursesMenuExtended() : 
	menu_new_opt_(0), menu_old_opt_(-1)
{
}

void CursesMenuExtended::process_menu_key(int key, vector<struct command> menu_vector)
{
	switch (key)
	{
	case 10:
	case 13:
	case KEY_ENTER:
		menu_old_opt_ = -1;
		erase();
		menu_vector[menu_new_opt_].function(window_);
		menu_new_opt_ = 0;
		break;

	case KEY_PPAGE:
	case KEY_HOME:
		menu_old_opt_ = menu_new_opt_;
		menu_new_opt_ = 0;
		break;

	case KEY_NPAGE:
	case KEY_END:
		menu_old_opt_ = menu_new_opt_;
		menu_new_opt_ = (int)menu_vector.size() - 1;
		break;

	case KEY_UP:
		menu_old_opt_ = menu_new_opt_;
		menu_new_opt_ = (menu_new_opt_ == 0) ?
			menu_new_opt_ : menu_new_opt_ - 1;
		break;

	case KEY_DOWN:
		menu_old_opt_ = menu_new_opt_;
		menu_new_opt_ = (menu_new_opt_ == menu_vector.size() - 1) ?
			menu_new_opt_ : menu_new_opt_ + 1;
		break;
	}
}

void CursesMenuExtended::display_menu(vector<struct command> options, string title)
{
	int menu_margin = 3;

	if (menu_old_opt_ == -1) {
		int i;

		attrset(A_BOLD);
		attron(COLOR_PAIR(get_color_schema_index()));
		mvaddstr_centered(1, title);
		attroff(COLOR_PAIR(get_color_schema_index()));
		attrset(A_NORMAL);

		//display_game_status(LINES - 5);
		//display_board_status(LINES - 4);

		for (i = 0; i < options.size(); i++)
			mvaddstr_centered(menu_margin + i, options[i].text);
	}
	else {
		mvaddstr_centered(menu_margin + menu_old_opt_, options[menu_old_opt_].text);
	}

	attrset(A_REVERSE);
	mvaddstr_centered(menu_margin + menu_new_opt_, options[menu_new_opt_].text);
	attrset(A_NORMAL);

	attrset(A_BOLD);
	attron(COLOR_PAIR(get_color_schema_index()));
	mvaddstr_centered(LINES - 3, "Use UP and DOWN Arrows to move and ENTER to select");
	attroff(COLOR_PAIR(get_color_schema_index()));
	attrset(A_NORMAL);
	refresh();
}