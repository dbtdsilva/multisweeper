#pragma once

#include "InterfaceVisual.h"

class Console : public InterfaceVisual
{
public:
	Console();
	virtual ~Console();

	virtual void gameStarted();
	virtual void gameFinished();
	virtual void boardPosRevealed(int x, int y, Position state);
	virtual void boardCreated(int height, int width);
	virtual void playerWon(Player player);
};

