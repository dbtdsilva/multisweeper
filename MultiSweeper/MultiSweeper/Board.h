#pragma once

#include <memory>
#include <list>
#include <vector>

using namespace std;

#include "BoardPosition.h"

class Board
{
public:	
	Board(int rows, int cols, int totalMines);
	virtual ~Board();

	void generateMines();
	void modifyBoard(int nRows, int nCols, int totalMines);
	list<BoardPosition *> revealPosition(int x, int y);
	bool allMinesRevealed();

	friend std::ostream& operator<<(std::ostream& os, const Board& obj);

private:
	list<BoardPosition *> revealFreePositions(int row, int col);
	void clearMines();

	std::vector<std::vector<std::unique_ptr<BoardPosition> > > mPos;
	int mRows, mCols, mTotalMines, mTotalMinesRevealed;
};
