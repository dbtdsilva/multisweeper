#include "Curses.h"
#include <locale.h>
#include <iostream>
#include <sstream>
#include <tuple>

using namespace std;
typedef struct command cmd;

Curses::Curses() :
	width_(10), height_(30), menu_new_opt_(0), menu_old_opt_(-1),
	color_schema_index_(2),	engine_(make_unique<Engine>(this, 10, 20, 10)),
	cols_(engine_->get_board().get_cols()), 
	rows_(engine_->get_board().get_rows()),
	mines_(engine_->get_board().get_total_mines()),
	mines_revealed_(engine_->get_board().get_total_mines_revealed()),
	player_list_(engine_->get_players_list()),
	options_main_({
		{ "Instructions", [=](WINDOW * win) { state_ = INSTRUCTIONS; } },
		{ "Adjust Board", [=](WINDOW * win) { state_ = BOARD_OPTIONS; } },
		{ "Manage Players", [=](WINDOW * win) { state_ = PLAYERS_OPTIONS; } },
		{ "Start Game", [=](WINDOW * win) { state_ = GAME; } },
		{ "Exit", [=](WINDOW * win) { state_ = EXIT_REQUEST; } } }),
	options_board_({
		{ "Rows", [=](WINDOW * win) { state_ = MODIFY_ROWS; } },
		{ "Columns", [=](WINDOW * win) { state_ = MODIFY_COLS; } },
		{ "Number of mines", [=](WINDOW * win) { state_ = MODIFY_MINES; } },
		{ "Return to Main Menu", [=](WINDOW * win) { state_ = MAIN_MENU; } } }),
	options_players_({
		{ "Add player", [=](WINDOW * win) { state_ = PLAYER_ADD; } },
		{ "Remove player", [=](WINDOW * win) { state_ = PLAYER_REMOVE; } },
		{ "Return to Main Menu", [=](WINDOW * win) { state_ = MAIN_MENU; } }}),
	state_(MAIN_MENU),
	game_is_running_(false)
{
	setlocale(LC_ALL, "");
	init();
}

Curses::~Curses()
{
}

void Curses::loop() 
{
	do {
		attron(COLOR_PAIR(color_schema_index_));
		border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, 
			ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
		attroff(COLOR_PAIR(color_schema_index_));
		display_curses();
		int key = getch();
		process_key(key);		
	} while (state_ != EXIT_REQUEST);

	delwin(window_);
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
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_YELLOW, COLOR_BLACK);
		init_pair(3, COLOR_CYAN, COLOR_BLACK);
		init_pair(4, COLOR_RED, COLOR_BLACK);
		init_pair(5, COLOR_GREEN, COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
		wbkgd(window_, COLOR_PAIR(1));
	} else {
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

void Curses::display_curses() 
{
	switch (state_) {
	case MAIN_MENU:
		display_menu(options_main_);
		break;
	case BOARD_OPTIONS:
		display_menu(options_board_);
		break;
	case INSTRUCTIONS:
		display_instructions();
		break;
	case PLAYERS_OPTIONS:
		display_menu(options_players_);
		break;
	case PLAYER_ADD:
		add_player();
		break;
	case PLAYER_REMOVE:
		remove_player();
		break;
	case MODIFY_COLS:
		modify_cols();
		break;
	case MODIFY_ROWS:
		modify_rows();
		break;
	case MODIFY_MINES:
		modify_mines();
		break;
	case GAME:
		display_game();
		break;
	}
}

void Curses::process_key(int key) 
{
	switch (state_) {
	case MAIN_MENU:
		process_menu_key(key, options_main_);
		break;
	case BOARD_OPTIONS:
		process_menu_key(key, options_board_);
		break;
	case PLAYERS_OPTIONS:
		process_menu_key(key, options_players_);
		break;
	case INSTRUCTIONS:
	case GAME:
		state_ = MAIN_MENU;	// Any key to continue
		erase();
		break;
	case PLAYER_ADD:
	case PLAYER_REMOVE:
		state_ = PLAYERS_OPTIONS;
		erase();
		break;
	case MODIFY_COLS:
	case MODIFY_ROWS:
	case MODIFY_MINES:
		state_ = BOARD_OPTIONS;	// Any key to continue
		erase();
		break;
	}
}

void Curses::display_instructions() 
{
	attrset(A_BOLD);
	mvaddstr_centered(1, "INSTRUCTIONS");
	attrset(A_NORMAL);

	mvaddstr_centered(3, "The objective of the game consists in DISCOVERING the MAXIMUM ");
	mvaddstr_centered(4, "number of MINES as possible. ");
	mvaddstr_centered(5, "There will be a board 2D with mines hidden on it. Until players");
	mvaddstr_centered(6, "find all the mines on the board, the game will not finish.");
	mvaddstr_centered(7, "On your turn, you might select a position to reveal.");

	attrset(A_BOLD);
	mvaddstr_centered(9, "Every square has a meaning:");
	attrset(A_NORMAL);
	// Left aligned and centered according to the biggest bullet item
	mvaddstr(10, (COLS - 62) / 2, "* Number on the square is the number of adjacent mines;");
	mvaddstr(11, (COLS - 62) / 2, "\tE.g. \"4\" means that there is 4 mines adjacent to that square");
	mvaddstr(12, (COLS - 62) / 2, "*");
	mvaddch(12, (COLS - 62) / 2 + 2, ACS_DIAMOND);
	mvaddstr(12, (COLS - 62) / 2 + 4, "is a revealed mine;");
	mvaddstr(13, (COLS - 62) / 2, "*");
	mvaddch(13, (COLS - 62) / 2 + 2, 250 | A_ALTCHARSET);
	mvaddstr(13, (COLS - 62) / 2 + 4, "appears when there the square was not revealed yet;");
	mvaddstr(14, (COLS - 62) / 2, "* If the square is empty, it has no adjacent mines to that square.");

	attrset(A_BOLD);
	attron(COLOR_PAIR(color_schema_index_));
	mvaddstr_centered(16, "Press any key to continue");
	attroff(COLOR_PAIR(color_schema_index_));
	attrset(A_NORMAL);
}

void Curses::display_board_status(int row) 
{
	stringstream ss;
	ss << "Board has " << rows_ << " rows and " << cols_ << " columns";
	ss << " with " << mines_ << " mines";
	mvaddstr_centered(row, ss.str());
}

void Curses::display_game_status(int row) 
{
	if (player_list_.empty()) {
		mvaddstr_centered(row, "There is no players registered yet");
	} else {
		stringstream ss;
		ss << player_list_.size() << " players registered";
		mvaddstr_centered(row, ss.str());
	}
}

void Curses::display_game() 
{
	engine_->start_game();
	int row_offset = 1;
	int game_window_offset = 3;
	int col_offset = (COLS - (cols_ * 2 + 2)) / 2;
	
	attron(COLOR_PAIR(color_schema_index_));
	mvaddch(game_window_offset - 1, col_offset - 1, ACS_ULCORNER);
	mvaddch(game_window_offset - 1, col_offset + cols_ * 2 + 1, ACS_URCORNER);
	mvaddch(game_window_offset + rows_, col_offset - 1, ACS_LLCORNER);
	mvaddch(game_window_offset + rows_, col_offset + cols_ * 2 + 1, ACS_LRCORNER);
	for (int row = 0; row < rows_; row++) {
		mvaddch(row + game_window_offset, col_offset - 1, ACS_VLINE);
		mvaddch(row + game_window_offset, col_offset + cols_ * 2 + 1, ACS_VLINE);
	}
	for (int col = 0; col < cols_ * 2 + 1; col++) {
		mvaddch(game_window_offset + rows_, col + col_offset, ACS_HLINE);
		mvaddch(game_window_offset - 1, col + col_offset, ACS_HLINE);
	}
	attroff(COLOR_PAIR(color_schema_index_));

	for (int row = 0; row < rows_; row++) {
		for (int col = 0; col < cols_; col++) {
			mvaddch(row + game_window_offset, 1 + col * 2 + col_offset, 250 | A_ALTCHARSET);
		}
	}

	tuple<int, int> new_position_selected = { 0, 0 };
	int& row = get<0>(new_position_selected);
	int& col = get<1>(new_position_selected);

	int key;
	stringstream ss;
	int const &current_player_index = engine_->get_current_player_index();

	string empty_string;
	for (int i = 0; i < COLS - 2; i++) {
		empty_string.append(" ");
	}

	bool current_player;
	while (game_is_running_) {
		represent_board_cursor(row, col, game_window_offset, col_offset);

		for (int player_index = 0; player_index < player_list_.size(); player_index++) {
			current_player = player_list_[player_index] == player_list_[current_player_index];
			if (current_player) {
				attron(COLOR_PAIR(color_schema_index_));
			}
			ss.str("");
			ss << player_list_[player_index].get_username();
			ss << "[ " << player_list_[player_index].
			mvaddstr_centered(LINES - player_list_.size() + player_index -1, empty_string);
			mvaddstr_centered(LINES - player_list_.size() + player_index - 1, ss.str());
			if (current_player) {
				attroff(COLOR_PAIR(color_schema_index_));
			}
		}
		ss.str("");
		ss << mines_revealed_ << " out of " << mines_ << " were revealed";
		mvaddstr_centered(row_offset, ss.str());
		key = getch();
		switch (key)
		{
		case 10:
		case 13:
		case KEY_ENTER:
			engine_->turn_played(row, col);
			break;
		case KEY_DOWN:
			row = row < rows_ - 1 ? row + 1 : row;
			break;
		case KEY_UP:
			row = row > 0 ? row - 1 : row;
			break;
		case KEY_LEFT:
			col = col > 0 ? col - 1 : col;
			break;
		case KEY_RIGHT:
			col = col < cols_ - 1? col + 1 : col;
			break;
		}
	}
}

void Curses::represent_board_cursor(int new_row, int new_col, int row_offset, int col_offset) 
{
	int& row = get<0>(board_position_selected_);
	int& col = get<1>(board_position_selected_);

	mvaddstr(row_offset + row, col_offset + col * 2, " ");
	mvaddstr(row_offset + row, col_offset + col * 2 + 2, " ");

	row = new_row;
	col = new_col;

	attron(COLOR_PAIR(color_schema_index_));
	mvaddstr(row_offset + row, col_offset + col * 2, "[");
	mvaddstr(row_offset + row, col_offset + col * 2 + 2, "]");
	attroff(COLOR_PAIR(color_schema_index_));
}

void Curses::game_started() 
{
	game_is_running_ = true;
}

void Curses::game_finished() 
{
	game_is_running_ = false;
}

void Curses::dispatch_error(const SweeperError& err) 
{
	display_error(2, err.get_message());
}

void Curses::display_error(int row, std::string message) 
{
	attrset(A_BOLD);
	attron(COLOR_PAIR(4));
	mvaddstr_centered(row, message);
	attroff(COLOR_PAIR(4));
	attrset(A_NORMAL);
	refresh();
}

void Curses::board_position_revealed(list<BoardPosition *> positions) 
{
	string position_character;
	for (BoardPosition * pos : positions) {
		tuple<int, int> const& position = pos->get_position();
		
		if (pos->is_mine()) {
			mvaddch(3 + get<0>(position),
				(COLS - (cols_ * 2 + 2)) / 2 + get<1>(position) * 2 + 1, ACS_DIAMOND);
		} else {
			int neighbour_mines = pos->get_count_neighbour_mines();
			position_character = neighbour_mines == 0 ? " " : to_string(neighbour_mines).c_str();

			attron(COLOR_PAIR(color_schema_index_));
			mvaddstr(3 + get<0>(position),
				(COLS - (cols_ * 2 + 2)) / 2 + get<1>(position) * 2 + 1, position_character.c_str());
			attroff(COLOR_PAIR(color_schema_index_));
		}
	}
}

void Curses::board_created(int height, int width) 
{
}

void Curses::player_won(Player player) 
{
}

void Curses::modify_rows() 
{
	int new_rows;
	display_board_status(1);
	try {
		mvscanw_robust("Enter the total number of ROWS", 3, &new_rows);
		if (new_rows <= 0) {
			display_error(2, "Number of rows inserted is invalid");
			return;
		}
		engine_->modify_board(new_rows, cols_, mines_);
	} catch (...) {
		display_error(2, "Received invalid input!");
	}
	display_board_status(7);
}

void Curses::modify_cols() 
{
	int new_cols;
	display_board_status(1);
	try {
		mvscanw_robust("Enter the total number of COLUMNS", 3, &new_cols);
		if (new_cols <= 0) {
			display_error(2, "Number of columns inserted is invalid");
			return;
		}
		engine_->modify_board(rows_, new_cols, mines_);
	} catch (...) {
		display_error(2, "Received invalid input!");
	}
	display_board_status(7);
}

void Curses::modify_mines() 
{
	int new_total_mines;
	display_board_status(1);
	try {
		mvscanw_robust("Enter the total number of MINES", 3, &new_total_mines);
		if (new_total_mines <= 0) {
			display_error(2, "Number of mines inserted is invalid");
			return;
		}
		engine_->modify_board(rows_, cols_, new_total_mines);
	} catch (...) {
		display_error(2, "Received invalid input!");
	}
	display_board_status(7);
}

void Curses::add_player() 
{
	string username;
	try {
		mvscanw_robust("Enter player's username", 3, &username);
		engine_->join_game(username.c_str());
	} catch (...) {
		display_error(2, "Received invalid input!");
	}
}

void Curses::remove_player() 
{
	if (player_list_.size() == 0) {
		display_error(2, "There is no players to remove");
		return;
	}
	attrset(A_BOLD);
	mvaddstr_centered(1, "Current player list");
	attrset(A_NORMAL);
	stringstream ss;
	for (int i = 0; i < player_list_.size(); i++) {
		ss.str("");
		ss << i << ": " << player_list_[i].get_username();
		mvaddstr(i + 3, (COLS - 20) / 2, ss.str().c_str());
	}

	int id;

	try {
		mvscanw_robust("Enter player's ID that you which to remove", (int)player_list_.size() + 4, &id);
		engine_->leave_game(id);
	} catch (...) {
		display_error(2, "Received invalid input");
	}
}

void Curses::mvaddstr_centered(int row, string message) 
{
	mvaddstr(row, (COLS - (int)message.size()) / 2, message.c_str());
}

void Curses::display_menu(vector<cmd> options)
{
	int menuMargin = 3;

	if (menu_old_opt_ == -1) {
		int i;

		attrset(A_BOLD);
		attron(COLOR_PAIR(color_schema_index_));
		mvaddstr_centered(1, "MultiSweeper Console");
		attroff(COLOR_PAIR(color_schema_index_));
		attrset(A_NORMAL);

		display_game_status(LINES - 5);
		display_board_status(LINES - 4);

		for (i = 0; i < options.size(); i++)
			mvaddstr_centered(menuMargin + i, options[i].text);
	} else {
		mvaddstr_centered(menuMargin + menu_old_opt_, options[menu_old_opt_].text);
	}

	attrset(A_REVERSE);
	mvaddstr_centered(menuMargin + menu_new_opt_, options[menu_new_opt_].text);
	attrset(A_NORMAL);

	attrset(A_BOLD);
	attron(COLOR_PAIR(color_schema_index_));
	mvaddstr_centered(LINES - 3, "Use UP and DOWN Arrows to move and ENTER to select");
	attroff(COLOR_PAIR(color_schema_index_));
	attrset(A_NORMAL);
	refresh();
}

void Curses::process_menu_key(int key, vector<cmd> menu_vector)
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

template<typename T>
void Curses::mvscanw_robust(string intro, int start_row, T* return_value)
{
	attrset(A_BOLD);
	mvaddstr_centered(start_row, intro);
	attrset(A_NORMAL);
	refresh();
	echo();
	curs_set(true);

	int valid_scan_result;
	if (is_same<T, int>::value) {
		valid_scan_result = mvscanw(start_row + 1, (COLS - 10) / 2, "%d", return_value);
	} else if (is_same<T, string>::value) {
		char word[31];
		valid_scan_result = mvscanw(start_row + 1, (COLS - (int)intro.size()) / 2, "%30s", word);
		stringstream ss(word);
		ss >> *return_value;
	}

	if (!valid_scan_result) {
		throw runtime_error("Invalid input received");
	}

	noecho();
	curs_set(false);
	attrset(A_BOLD);
	attron(COLOR_PAIR(color_schema_index_));
	mvaddstr_centered(start_row + 3, "Press any key to continue");
	attroff(COLOR_PAIR(color_schema_index_));
	attrset(A_NORMAL);
}