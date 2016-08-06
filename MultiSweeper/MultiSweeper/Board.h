#pragma once

#include <memory>
#include <vector>

using namespace std;
class Board
{
public:	
	Board(int width, int height, int totalMines);
	virtual ~Board();

	void generateMines();
	void modifyBoard(int width, int height, int totalMines);
	friend std::ostream& operator<<(std::ostream& os, const Board& obj);

	enum Position {
		FREE,
		MINE,
		FREE_DISCOVERED,
		MINE_DISCOVERED
	};

private:
	void clearMines();

	std::vector<std::vector<Position> > mPos;
	int mHeight;
	int mWidth;
	int mTotalMines;

};
