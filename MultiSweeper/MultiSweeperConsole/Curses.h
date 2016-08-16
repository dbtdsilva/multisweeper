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
		MODIFY_LINES,
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

	void mvaddstrCentered(int row, string str);

	std::unique_ptr<Console> pSweeperConsole;
	std::unique_ptr<Engine> pEngine;
	VisualState state;
	std::vector<cmd> mMainuOptions, mBoardOptions, mPlayersOptions;
	int old_option, new_option;
	int height, width;
	WINDOW *window;
};

