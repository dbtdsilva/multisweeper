#pragma once

#include <memory>
#include "Engine.h"
#include "InterfaceVisual.h"
#include "CursesMenuExtended.h"
#include "SweeperError.h"
#include "Player.h"
#include "BoardPosition.h"

class SweeperCurses : public CursesMenuExtended, public InterfaceVisual
{
public:
	SweeperCurses();
	~SweeperCurses();

	void loop();
	virtual void game_started();
	virtual void game_finished();
	virtual void board_position_revealed(std::list<BoardPosition *> states);
	virtual void board_created(int height, int width);
	virtual void player_won(std::vector<const Player*> players);
	virtual void dispatch_error(const SweeperError& error);
private:
	void display_curses();
	void process_key(int key);
	void display_instructions();
	void represent_board_cursor(int new_row, int new_col);
	void modify_rows();
	void modify_cols();
	void modify_mines();
	void add_player();
	void remove_player();
	void display_board_status(int row);
	void display_game_status(int row);
	void display_game();
	
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

	VisualState state_;
	std::vector<struct command> options_main_, options_board_, options_players_;
	std::tuple<int, int> board_position_selected_;
	std::unique_ptr<Engine> engine_;
	int const &mines_, &rows_, &cols_, &mines_revealed_;
	int error_offset_;
	int game_offset_row_;
	bool game_is_running_;
	std::vector<Player> const& player_list_;
};

