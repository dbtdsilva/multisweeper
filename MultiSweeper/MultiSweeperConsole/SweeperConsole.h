#pragma once

#include "InterfaceVisual.h"

class SweeperConsole : public InterfaceVisual
{
public:
	SweeperConsole();
	virtual ~SweeperConsole();

	virtual void gameStarted();
	virtual void gameFinished();
	virtual void boardPosRevealed(int x, int y, Position state);
	virtual void boardCreated(int height, int width);
	virtual void playerWon(Player player);
};

