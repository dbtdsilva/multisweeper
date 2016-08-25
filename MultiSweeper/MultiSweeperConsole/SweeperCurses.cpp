#include "SweeperCurses.h"

#include <sstream>

using namespace std;

SweeperCurses::SweeperCurses() :
	engine_(make_unique<Engine>(this, 10, 20, 10)),
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
		{ "Return to Main Menu", [=](WINDOW * win) { state_ = MAIN_MENU; } } }),
	state_(MAIN_MENU), game_is_running_(false), error_offset_(2), game_offset_row_(4)
{
}

SweeperCurses::~SweeperCurses()
{
}

void SweeperCurses::loop()
{
	do {
		attron(COLOR_PAIR(get_color_schema_index()));
		border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
			ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
		attroff(COLOR_PAIR(get_color_schema_index()));
		display_curses();
		int key = getch();
		process_key(key);
	} while (state_ != EXIT_REQUEST);

	delwin(window_);
	endwin();
}

void SweeperCurses::display_curses()
{
	switch (state_) {
	case MAIN_MENU:
		display_menu(options_main_, "Multisweeper Console");
		break;
	case BOARD_OPTIONS:
		display_menu(options_board_, "Multisweeper Console");
		break;
	case INSTRUCTIONS:
		display_instructions();
		break;
	case PLAYERS_OPTIONS:
		display_menu(options_players_, "Multisweeper Console");
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

void SweeperCurses::process_key(int key)
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

void SweeperCurses::display_instructions()
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
	attron(COLOR_PAIR(get_color_schema_index()));
	mvaddstr_centered(16, "Press any key to continue");
	attroff(COLOR_PAIR(get_color_schema_index()));
	attrset(A_NORMAL);
}

void SweeperCurses::display_board_status(int row)
{
	stringstream ss;
	ss << "Board has " << rows_ << " rows and " << cols_ << " columns";
	ss << " with " << mines_ << " mines";
	mvaddstr_centered(row, ss.str());
}

void SweeperCurses::display_game_status(int row)
{
	if (player_list_.empty()) {
		mvaddstr_centered(row, "There is no players registered yet");
	}
	else {
		stringstream ss;
		ss << player_list_.size() << " players registered";
		mvaddstr_centered(row, ss.str());
	}
}

void SweeperCurses::display_game()
{
	engine_->start_game();

	int game_offset_col = ((COLS - (cols_ * 2 + 2)) / 2);
	attron(COLOR_PAIR(get_color_schema_index()));
	mvaddch(game_offset_row_ - 1, game_offset_col - 1, ACS_ULCORNER);
	mvaddch(game_offset_row_ - 1, game_offset_col + cols_ * 2 + 1, ACS_URCORNER);
	mvaddch(game_offset_row_ + rows_, game_offset_col - 1, ACS_LLCORNER);
	mvaddch(game_offset_row_ + rows_, game_offset_col + cols_ * 2 + 1, ACS_LRCORNER);
	for (int row = 0; row < rows_; row++) {
		mvaddch(row + game_offset_row_, game_offset_col - 1, ACS_VLINE);
		mvaddch(row + game_offset_row_, game_offset_col + cols_ * 2 + 1, ACS_VLINE);
	}
	for (int col = 0; col < cols_ * 2 + 1; col++) {
		mvaddch(game_offset_row_ + rows_, col + game_offset_col, ACS_HLINE);
		mvaddch(game_offset_row_ - 1, col + game_offset_col, ACS_HLINE);
	}
	attroff(COLOR_PAIR(get_color_schema_index()));

	for (int row = 0; row < rows_; row++) {
		for (int col = 0; col < cols_; col++) {
			mvaddch(row + game_offset_row_, 1 + col * 2 + game_offset_col, 250 | A_ALTCHARSET);
		}
	}

	tuple<int, int> new_position_selected = { 0, 0 };
	int& row = get<0>(new_position_selected);
	int& col = get<1>(new_position_selected);

	int key;
	stringstream ss;
	int const &current_player_index = engine_->get_current_player_index();
	
	bool current_player;
	while (game_is_running_) {
		represent_board_cursor(row, col);

		for (int player_index = 0; player_index < player_list_.size(); player_index++) {
			current_player = player_list_[player_index] == player_list_[current_player_index];
			if (current_player) {
				attron(COLOR_PAIR(get_color_schema_index()));
			}
			ss.str("");
			ss << player_list_[player_index].get_username();
			ss << " [ Mines found: " << player_list_[player_index].get_mines_revealed() << " ]";
			clear_specific(LINES - (int)player_list_.size() + player_index - 1, COLS - 2);
			mvaddstr_centered(LINES - (int)player_list_.size() + player_index - 1, ss.str());
			if (current_player) {
				attroff(COLOR_PAIR(get_color_schema_index()));
			}
		}
		ss.str("");
		ss << mines_revealed_ << " out of " << mines_ << " were revealed";
		mvaddstr_centered(game_offset_row_ - 3, ss.str());
		key = getch();
		switch (key)
		{
		case 10:
		case 13:
		case KEY_ENTER:
			clear_specific(error_offset_, COLS - 2);
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
			col = col < cols_ - 1 ? col + 1 : col;
			break;
		}
	}
}

void SweeperCurses::represent_board_cursor(int new_row, int new_col)
{
	int game_offset_col = ((COLS - (cols_ * 2 + 2)) / 2);
	int& row = get<0>(board_position_selected_);
	int& col = get<1>(board_position_selected_);
	mvaddstr(game_offset_row_ + row, game_offset_col + col * 2, " ");
	mvaddstr(game_offset_row_ + row, game_offset_col + col * 2 + 2, " ");

	row = new_row;
	col = new_col;

	attron(COLOR_PAIR(get_color_schema_index()));
	mvaddstr(game_offset_row_ + row, game_offset_col + col * 2, "[");
	mvaddstr(game_offset_row_ + row, game_offset_col + col * 2 + 2, "]");
	attroff(COLOR_PAIR(get_color_schema_index()));
}

void SweeperCurses::game_started()
{
	game_is_running_ = true;
}

void SweeperCurses::game_finished()
{
	game_is_running_ = false;
}

void SweeperCurses::dispatch_error(const SweeperError& err)
{
	display_error(error_offset_, err.get_message());
}

void SweeperCurses::board_position_revealed(list<BoardPosition *> positions)
{
	int game_offset_col = ((COLS - (cols_ * 2 + 2)) / 2);
	string position_character;
	for (BoardPosition * pos : positions) {
		tuple<int, int> const& position = pos->get_position();

		if (pos->is_mine()) {
			mvaddch(game_offset_row_ + get<0>(position), 
				game_offset_col + get<1>(position) * 2 + 1, ACS_DIAMOND);
		}
		else {
			int neighbour_mines = pos->get_count_neighbour_mines();
			position_character = neighbour_mines == 0 ? " " : to_string(neighbour_mines).c_str();

			attron(COLOR_PAIR(get_color_schema_index()));
			mvaddstr(game_offset_row_ + get<0>(position),
				game_offset_col + get<1>(position) * 2 + 1, position_character.c_str());
			attroff(COLOR_PAIR(get_color_schema_index()));
		}
	}
}

void SweeperCurses::board_created(int height, int width)
{
}

void SweeperCurses::player_won(vector<const Player*> players)
{
}

void SweeperCurses::modify_rows()
{
	int new_rows;
	display_board_status(1);
	try {
		mvscanw_robust("Enter the total number of ROWS", 3, &new_rows);
		if (new_rows <= 0 || new_rows >= (LINES - engine_->get_max_players() - game_offset_row_ - 1)) {
			display_error(error_offset_, "Number of rows inserted is invalid");
			return;
		}
		engine_->modify_board(new_rows, cols_, mines_);
	}
	catch (...) {
		display_error(error_offset_, "Received invalid input!");
	}
	display_board_status(7);
}

void SweeperCurses::modify_cols()
{
	int new_cols;
	display_board_status(1);
	try {
		mvscanw_robust("Enter the total number of COLUMNS", 3, &new_cols);
		if (new_cols <= 0 || new_cols >= COLS / 2 - 2) {
			display_error(error_offset_, "Number of columns inserted is invalid");
			return;
		}
		engine_->modify_board(rows_, new_cols, mines_);
	}
	catch (...) {
		display_error(2, "Received invalid input!");
	}
	display_board_status(7);
}

void SweeperCurses::modify_mines()
{
	int new_total_mines;
	display_board_status(1);
	try {
		mvscanw_robust("Enter the total number of MINES", 3, &new_total_mines);
		if (new_total_mines <= 0) {
			display_error(error_offset_, "Number of mines inserted is invalid");
			return;
		}
		engine_->modify_board(rows_, cols_, new_total_mines);
	}
	catch (...) {
		display_error(error_offset_, "Received invalid input!");
	}
	display_board_status(7);
}

void SweeperCurses::add_player()
{
	string username;
	try {
		mvscanw_robust("Enter player's username", 3, &username);
		engine_->join_game(username.c_str());
	}
	catch (...) {
		display_error(error_offset_, "Received invalid input!");
	}
}

void SweeperCurses::remove_player()
{
	if (player_list_.size() == 0) {
		display_error(error_offset_, "There is no players to remove");
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
	}
	catch (...) {
		display_error(error_offset_, "Received invalid input");
	}
}
