#pragma once

#include <memory>
#include <vector>

using namespace std;
class Board
{
public:	
	Board(int width, int height, int nmines);
	virtual ~Board();
	friend std::ostream& operator<<(std::ostream& os, const Board& obj);

private:
	enum Position {
		FREE,
		MINE,
		FREE_DISCOVERED,
		MINE_DISCOVERED,
		MINE_DESTROYED
	};

	std::vector<std::vector<Position> > mPos;
	int mHeight;
	int mWidth;
	int mTotalMines;

};
