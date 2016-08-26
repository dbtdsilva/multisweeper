/*! \brief Engine class represents the MultiSweeper game engine providing an interface
*  for any visual framework that wants to use it.
*
*  This class represents the MultiSweeper game engine providing an interface
*  for any visual framework that wants to use it. All errors are reported to that
*  interface. Engine currently supports error handling, players, game stats and the
*  game itself.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#pragma once

#include "Board.h"
#include "InterfaceVisual.h"
#include <string>
#include <vector>

class Engine
{
public:
	Engine(InterfaceVisual* interaction);
	Engine(InterfaceVisual* interaction, int rows, int cols, int mines);
	Engine(const Engine & other);
	Engine(Engine&& other);
	virtual ~Engine();

	Engine& operator=(const Engine& other);
	Engine& operator=(Engine&& other);

	void start_game();
	void modify_board(int rows, int cols, int total_mines);
	void join_game(std::string username);
	void leave_game(std::string username);
	void leave_game(int id);
	void turn_played(int row, int col, bool special_bomb = false);
	void surrender();

	int const& get_current_player_index();
	Board const& get_board() const;
	std::vector<Player> const& get_players_list() const;
	int const& get_max_players() const;

	friend std::ostream& operator<<(std::ostream& os, const Engine& obj);
private:
	void game_finished();
	Player & next_player();

	enum Status {
		START,
		RUN
	};

	bool is_game_finished();
	bool verify_game_status(Status expected);

	const int kMaxPlayers;
	Status current_status_;

	int current_player_index_;
	InterfaceVisual* interaction_;
	std::vector<Player> players_;
	std::unique_ptr<Board> board_;
};

