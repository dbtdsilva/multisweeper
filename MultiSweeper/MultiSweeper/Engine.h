#pragma once

#include "Board.h"
#include <string>

class Engine
{
public:
	Engine();
	virtual ~Engine();

	enum Status {
		START,
		RUN,
		FINISH
	};
	static int MAX_PLAYERS;

	void startGame();
	void joinGame(std::string username);
	void getStatus() const;
	friend std::ostream& operator<<(std::ostream& os, const Engine& obj);
private:
	Status currentStatus;
	unsigned int currentNumPlayers;
	std::unique_ptr<Board> mBoard;
};

