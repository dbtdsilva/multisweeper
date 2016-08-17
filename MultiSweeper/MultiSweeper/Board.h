#pragma once

#include <memory>
#include <vector>

using namespace std;

enum Position {
	FREE,
	MINE
};

struct BoardPos {
	Position state{ FREE };
	bool revealed{ false };
};

class Board
{
public:	
	Board(int rows, int cols, int totalMines);
	virtual ~Board();

	void generateMines();
	void modifyBoard(int nRows, int nCols, int totalMines);
	void modifyBoard(int nRows, int nCols);
	void modifyNumberMines(int totalMines);
	Position revealPosition(int x, int y);
	bool allMinesRevealed();

	friend std::ostream& operator<<(std::ostream& os, const Board& obj);

private:
	void clearMines();

	std::vector<std::vector<BoardPos> > mPos;
	int mRows, mCols, mTotalMines, mTotalMinesRevealed;
};
