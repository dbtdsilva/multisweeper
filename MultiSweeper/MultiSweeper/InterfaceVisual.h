#pragma once

#include <list>
#include "SweeperError.h"
#include "BoardPosition.h"
#include "Player.h"

class InterfaceVisual
{
public:
	InterfaceVisual();
	virtual ~InterfaceVisual();

	virtual void gameStarted() = 0;
	virtual void gameFinished() = 0;
	virtual void boardPosRevealed(std::list<BoardPosition *> states) = 0;
	virtual void boardCreated(int height, int width) = 0;
	virtual void playerWon(Player player) = 0;
	virtual void errorOccured(const SweeperError& error);
};

