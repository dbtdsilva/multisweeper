#pragma once

#include <list>
#include <vector>
#include "SweeperError.h"
#include "BoardPosition.h"
#include "Player.h"

class InterfaceVisual
{
public:
	InterfaceVisual();
	virtual ~InterfaceVisual();

	virtual void game_started() = 0;
	virtual void game_finished() = 0;
	virtual void board_position_revealed(std::list<BoardPosition *> states) = 0;
	virtual void board_created(int height, int width) = 0;
	virtual void player_won(vector<const Player*> players) = 0;
	virtual void dispatch_error(const SweeperError& error);
};

