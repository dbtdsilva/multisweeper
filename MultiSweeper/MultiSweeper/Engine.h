#pragma once

#include "Board.h"
#include "InterfaceVisual.h"
#include <string>
#include <vector>

class Engine
{
public:
	Engine();
	Engine(InterfaceVisual* iv);
	Engine(InterfaceVisual* iv, int nRows, int nCols, int nMines);
	virtual ~Engine();

	void start_game();
	void modify_board(int nRows, int nCols, int nTotalMines);
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
	Status currentStatus;

	int currentPlayerIdx;
	Player* currentPlayer;
	InterfaceVisual* mInteraction;
	std::vector<Player> mPlayers;
	std::unique_ptr<Board> mBoard;
};

