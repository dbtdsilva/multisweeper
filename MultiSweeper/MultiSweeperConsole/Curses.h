#pragma once

#include <curses.h>
#include <vector>

class Curses
{
public:
	Curses();
	virtual ~Curses();

	void loop();
private:

	struct command
	{
		const char *text;
		void (*function)(WINDOW *);
	};
	typedef struct command cmd;

	void init();
	void paintMenu(std::vector<cmd> options, int, int);
	void displayMenu(std::vector<cmd> vec);

	int key, old_option = -1, new_option = 0;
	int height, width;
	bool quit = FALSE;
	WINDOW *win;
};

