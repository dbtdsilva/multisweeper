#pragma once

#include "InterfaceVisual.h"
#include <curses.h>

class Console : public InterfaceVisual
{
public:
	Console(WINDOW * window);
	virtual ~Console();

	virtual void gameStarted();
	virtual void gameFinished();
	virtual void boardPosRevealed(int x, int y, Position state);
	virtual void boardCreated(int height, int width);
	virtual void playerWon(Player player);

private:
	WINDOW * window;
};

