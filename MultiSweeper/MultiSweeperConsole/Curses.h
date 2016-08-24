#pragma once

#include <curses.h>
#include <vector>
#include <functional>
#include "Engine.h"

class Curses : public InterfaceVisual
{
public:
	Curses();
	virtual ~Curses();

	void loop();
	// InterfaceVirtual related
	virtual void game_started();
	virtual void game_finished();
	virtual void board_position_revealed(std::list<BoardPosition *> states);
	virtual void board_created(int height, int width);
	virtual void player_won(vector<const Player*> players);
	virtual void dispatch_error(const SweeperError& error);
private:
	struct command
	{
		std::string text;
		std::function<void(WINDOW *)> function;
	};
	typedef struct command cmd;

	enum VisualState {
		MAIN_MENU,
		BOARD_OPTIONS,
		INSTRUCTIONS,
		PLAYERS_OPTIONS,
		PLAYER_ADD,
		PLAYER_REMOVE,
		MODIFY_COLS,
		MODIFY_ROWS,
		MODIFY_MINES,
		GAME,
		EXIT_REQUEST
	};

	void init();
	void display_curses();
	void process_key(int key);
	void display_menu(std::vector<cmd> options);
	void display_instructions();
	void process_menu_key(int key, std::vector<cmd> menu_vector);
	void represent_board_cursor(int new_row, int new_col, int row_offset, int col_offset);
	void modify_rows();
	void modify_cols();
	void modify_mines();
	void add_player();
	void remove_player();
	void display_board_status(int row);
	void display_game_status(int row);
	void display_game();
	void display_error(int row, std::string message);

	template <typename T>
	void mvscanw_robust(string intro, int start_row, T * return_value);
	void mvaddstr_centered(int row, string message);

	VisualState state_;
	std::vector<cmd> options_main_, options_board_, options_players_;
	std::tuple<int, int> board_position_selected_;
	int menu_old_opt_, menu_new_opt_;
	int height_, width_;
	WINDOW *window_;
	std::unique_ptr<Engine> engine_;
	int const &mines_, &rows_, &cols_, &mines_revealed_;
	bool game_is_running_;
	vector<Player> const& player_list_;
	int color_schema_index_;
};
