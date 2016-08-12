#pragma once

#include <curses.h>
#include <vector>

class SweeperCurses
{
public:
	SweeperCurses();
	virtual ~SweeperCurses();

	void loop();
private:

	struct command
	{
		const char *text;
		void (*function)(WINDOW *);
	};
	typedef struct command cmd;

	void init();
	void paintMenu(int, int);
	void displayMenu(std::vector<cmd> vec);

	int key, old_option = -1, new_option = 0;
	bool quit = FALSE;
	WINDOW *win;
};

