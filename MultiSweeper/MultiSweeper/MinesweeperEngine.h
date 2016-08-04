#pragma once

class MinesweeperEngine
{
public:
	MinesweeperEngine();
	virtual ~MinesweeperEngine();

	enum MinesweeperStatus {
		START,
		RUN,
		FINISH
	};
	static int MAX_PLAYERS;

private:
	MinesweeperStatus currentStatus;
};

