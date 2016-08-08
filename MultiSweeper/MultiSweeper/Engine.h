#pragma once

#include "Board.h"
#include "InterfaceVisual.h"
#include <string>
#include <vector>

class Engine
{
public:
	Engine();
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
		RUN,
		FINISH
	};

	static int MAX_PLAYERS;
	Status currentStatus;

	int currentPlayerIdx;
	Player* currentPlayer;
	std::vector<Player> mPlayers;
	std::unique_ptr<Board> mBoard;
	std::unique_ptr<InterfaceVisual> mInteraction;
};

