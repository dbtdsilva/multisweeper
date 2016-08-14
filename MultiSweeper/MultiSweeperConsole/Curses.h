#pragma once

#include <curses.h>
#include <vector>
#include <functional>

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
		PLAYERS,
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
	void processMenuKey(int key, std::vector<cmd> vec);

	VisualState state;
	std::vector<cmd> mMainuOptions, mBoardOptions;
	int old_option = -1, new_option = 0;
	unsigned int height, width;
	WINDOW *win;
};

