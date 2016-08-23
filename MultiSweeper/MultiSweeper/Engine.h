#pragma once

#include "Board.h"
#include "InterfaceVisual.h"
#include <string>
#include <vector>

class Engine
{
public:
	Engine();
	Engine(InterfaceVisual* interaction);
	Engine(InterfaceVisual* interaction, int rows, int cols, int mines);
	virtual ~Engine();

	void start_game();
	void modify_board(int rows, int cols, int total_mines);
	void join_game(std::string username);
	void leave_game(std::string username);
	void leave_game(int id);
	void turn_played(int row, int col);
	void surrender(Player player);

	Player const& get_current_player() const;
	std::vector<Player> const& get_players_list() const;

	friend std::ostream& operator<<(std::ostream& os, const Engine& obj);
private:
	Player* next_player();

	enum Status {
		START,
		RUN
	};

	bool is_game_finished();
	bool verify_game_status(Status expected);

	const int kMaxPlayers;
	Status current_status_;

	int current_player_index_;
	Player* current_player_;
	InterfaceVisual* interaction_;
	std::vector<Player> players_;
	std::unique_ptr<Board> board_;
};

