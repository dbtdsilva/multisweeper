#pragma once

#include <curses.h>
#include <vector>
#include <functional>
#include "Engine.h"
#include "Console.h"

class Curses
{
public:
	Curses();
	virtual ~Curses();

	void loop();
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
	void modifyRows();
	void modifyCols();
	void modifyMines();
	void displayBoardStatus(int row);

	template <typename T>
	void mvscanwRobust(string introText, int rowStart, T * returnValue);
	void mvaddstrCentered(int row, string str);

	// Game related
	int mRows, mCols, mMines;
	// Curses related
	std::unique_ptr<Console> pSweeperConsole;
	std::unique_ptr<Engine> pEngine;
	VisualState state;
	std::vector<cmd> mMainuOptions, mBoardOptions, mPlayersOptions;
	int old_option, new_option;
	int height, width;
	WINDOW *window;
};

template<typename T>
inline void Curses::mvscanwRobust(string introText, int rowStart, T * returnValue)
{
	attrset(A_BOLD);
	mvaddstrCentered(rowStart, introText);
	attrset(A_NORMAL);
	refresh();
	echo();
	curs_set(true);
	if (is_same<T, int>::value) {
		mvscanw(rowStart + 1, (COLS - 2) / 2, "%d", returnValue);
	}
	else if (is_same<T, string>::value) {
		mvscanw(rowStart + 1, (COLS - 10)/ 2, "%s", returnValue);
	}
	else {
		throw runtime_error("Invalid type received into the function readValue");
	}
	noecho();
	curs_set(false);
	attrset(A_BOLD);
	mvaddstrCentered(rowStart + 4, "Press any key to continue");
	attrset(A_NORMAL);
}