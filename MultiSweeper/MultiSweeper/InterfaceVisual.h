#pragma once
#include "Board.h"
#include "Player.h"

class InterfaceVisual
{
public:
	InterfaceVisual();
	virtual ~InterfaceVisual();

	virtual void gameStarted() = 0;
	virtual void gameFinished() = 0;
	virtual void boardPosRevealed(int x, int y, Board::Position newStatus) = 0;
	virtual void boardCreated(int height, int width) = 0;
	virtual void playerWon(Player player) = 0;
};

