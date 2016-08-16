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

	void startGame();
	void joinGame(const std::string username);
	void turnPlayed(int x, int y);
	void surrender(Player player);
	Player* getCurrentPlayer();

	friend std::ostream& operator<<(std::ostream& os, const Engine& obj);
private:
	bool isGameFinished();
	Player* nextPlayer();

	enum Status {
		START,
		RUN
	};

	static int MAX_PLAYERS;
	Status currentStatus;

	int currentPlayerIdx;
	Player* currentPlayer;
	InterfaceVisual* mInteraction;
	std::vector<Player> mPlayers;
	std::unique_ptr<Board> mBoard;
};

