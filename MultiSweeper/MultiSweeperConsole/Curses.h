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
	virtual void gameStarted();
	virtual void gameFinished();
	virtual void boardPosRevealed(list<BoardPosition *> states);
	virtual void boardCreated(int height, int width);
	virtual void playerWon(Player player);
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
	void displayCurses();
	void processKey(int key);
	void displayMenu(std::vector<cmd> options);
	void displayInstructions();
	void processMenuKey(int key, std::vector<cmd> vec);
	void representBoardCursor(int new_row, int new_col);
	void modifyRows();
	void modifyCols();
	void modifyMines();
	void addPlayer();
	void removePlayer();
	void displayBoardStatus(int row);
	void displayGameStatus(int row);
	void displayGame();

	template <typename T>
	void mvscanwRobust(string introText, int rowStart, T * returnValue);
	void mvaddstrCentered(int row, string str);

	int mRows, mCols, mMines;
	VisualState state;
	std::vector<cmd> mMainuOptions, mBoardOptions, mPlayersOptions;
	std::tuple<int, int> boardPosSelected;
	int old_option, new_option;
	int height, width;
	WINDOW *window;
	std::unique_ptr<Engine> pEngine;
	bool gameIsRunning;
	vector<Player> const& playerList;
};
