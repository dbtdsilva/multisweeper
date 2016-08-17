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
	void modifyBoard(int nRows, int nCols);
	void modifyNumberMines(int nMines);
	void joinGame(std::string username);
	void leaveGame(std::string username);
	void leaveGame(int id);
	void turnPlayed(int x, int y);
	void surrender(Player player);

	Player const& getCurrentPlayer() const;
	std::vector<Player> const& getPlayersList() const;

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

