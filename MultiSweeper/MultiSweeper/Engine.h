#pragma once

#include "Board.h"
#include "InterfaceVisual.h"
#include <string>

class Engine
{
public:
	Engine();
	virtual ~Engine();

	void startGame();
	void joinGame(const std::string username);
	void turnPlayed(int x, int y);
	void surrender(Player player);
	friend std::ostream& operator<<(std::ostream& os, const Engine& obj);
private:
	enum Status {
		START,
		RUN,
		FINISH
	};

	static int MAX_PLAYERS;
	Status currentStatus;
	std::vector<Player> mPlayers;
	std::unique_ptr<Board> mBoard;
	std::unique_ptr<InterfaceVisual> mInteraction;
};

