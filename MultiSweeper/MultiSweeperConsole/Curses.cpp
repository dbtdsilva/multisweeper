#include "Curses.h"
#include <locale.h>
#include <iostream>
#include <sstream>
#include <tuple>

using namespace std;
typedef struct command cmd;

Curses::Curses() :
	width_(120), height_(30), menu_new_opt_(0), menu_old_opt_(-1),
	cols_(20), rows_(10), mines_(10),
	engine_(make_unique<Engine>(this, rows_, cols_, mines_)),
	player_list_(engine_->get_players_list()),
	options_main_({
		{ "Instructions", [=](WINDOW * win) { state_ = INSTRUCTIONS; } },
		{ "Modify Board", [=](WINDOW * win) { state_ = BOARD_OPTIONS; } },
		{ "Add/Remove Players", [=](WINDOW * win) { state_ = PLAYERS_OPTIONS; } },
		{ "Start Game", [=](WINDOW * win) { state_ = GAME; } },
		{ "Exit", [=](WINDOW * win) { state_ = EXIT_REQUEST; } } }),
	options_board_({
		{ "Change rows", [=](WINDOW * win) { state_ = MODIFY_ROWS; } },
		{ "Change lines", [=](WINDOW * win) { state_ = MODIFY_COLS; } },
		{ "Change number of mines", [=](WINDOW * win) { state_ = MODIFY_MINES; } },
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
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
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
	mvaddstr(12, (COLS - 62) / 2, "* \"X\" is a revealed mine;");
	mvaddstr(13, (COLS - 62) / 2, "* \"O\" appears when there is no adjacent mines to this square;");
	mvaddstr(14, (COLS - 62) / 2, "* If the square is empty, it has not been revealed yet;");

	attrset(A_BOLD);
	mvaddstr_centered(16, "Press any key to continue");
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

	tuple<int, int> newPosSelected = { 0, 0 };
	int& row = get<0>(newPosSelected);
	int& col = get<1>(newPosSelected);

	int key;
	stringstream ss;
	Player const& p = engine_->get_current_player();
	while (game_is_running_) {
		represent_board_cursor(row, col, 2, (COLS - (cols_ * 2 + 2)) / 2);
		ss.str("");
		ss << "Current player: " << p.get_username();
		mvaddstr_centered(0, ss.str());
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

	mvaddstr(row_offset + row, col_offset + col * 2, "[");
	mvaddstr(row_offset + row, col_offset + col * 2 + 2, "]");
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
	display_error(err.get_error_code(), err.get_message());
}

void Curses::display_error(int row, std::string message) 
{
	attrset(A_BOLD);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	mvaddstr_centered(row, message);
	attroff(COLOR_PAIR(1));
	attrset(A_NORMAL);
	refresh();
}

void Curses::board_position_revealed(list<BoardPosition *> positions) 
{
	for (BoardPosition * pos : positions) {
		tuple<int, int> const& position = pos->get_position();
		mvaddstr(2 + get<0>(position), 
			(COLS - (cols_ * 2 + 2)) / 2 + get<1>(position) * 2 + 1, pos->is_mine() ?
			"X" : to_string(pos->get_count_neighbour_mines()).c_str());
	}
}

void Curses::board_created(int height, int width) 
{
	cout << "Board created" << endl;
}

void Curses::player_won(Player player) 
{
	cout << player.get_username() << " has won" << endl;
}

void Curses::modify_rows() 
{
	int nRows;
	display_board_status(1);
	mvscanw_robust("Enter the total number of ROWS", 3, &nRows);
	if (nRows <= 0 || nRows >= rows_) {
		display_error(3, "Number of rows inserted is invalid");
		return;
	}
	this->rows_ = nRows;
	engine_->modify_board(rows_, cols_, mines_);
	display_board_status(6);
}

void Curses::modify_cols() 
{
	int nCols;
	display_board_status(1);
	mvscanw_robust("Enter the total number of COLUMNS", 3, &nCols);
	this->cols_ = nCols;
	engine_->modify_board(rows_, cols_, mines_);
	display_board_status(6);
}

void Curses::modify_mines() 
{
	int nMines;
	display_board_status(1);
	mvscanw_robust("Enter the total number of MINES", 3, &nMines);
	this->mines_ = nMines;
	engine_->modify_board(rows_, cols_, mines_);
	display_board_status(6);
}

void Curses::add_player() 
{
	string username;
	mvscanw_robust("Enter player's username", 3, &username);
	engine_->join_game(username.c_str());
}

void Curses::remove_player() 
{
	attrset(A_BOLD);
	mvaddstr_centered(1, "Current player list");
	attrset(A_NORMAL);
	stringstream ss;
	for (int i = 0; i < player_list_.size(); i++) {
		ss.str("");
		ss << i << ": " << player_list_[i].get_username();
		mvaddstr(i + 2, (COLS - 20) / 2, ss.str().c_str());
	}

	int id;
	mvscanw_robust("Enter player's ID that you which to remove", (int)player_list_.size() + 3, &id);
	engine_->leave_game(id);
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
		mvaddstr_centered(1, "MultiSweeper Console");
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
	mvaddstr_centered(LINES - 3, " > Use UP and DOWN Arrows to move and ENTER to select <");
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
	if (is_same<T, int>::value) {
		mvscanw(start_row + 1, (COLS - 2) / 2, "%d", return_value);
	}
	else if (is_same<T, string>::value) {
		mvscanw(start_row + 1, (COLS - (int)intro.size()) / 2, "%s", return_value);
	}
	else {
		throw runtime_error("Invalid type received into the function readValue");
	}
	noecho();
	curs_set(false);
	attrset(A_BOLD);
	mvaddstr_centered(start_row + 4, "Press any key to continue");
	attrset(A_NORMAL);
}