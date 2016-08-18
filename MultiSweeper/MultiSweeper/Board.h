#pragma once

#include <memory>
#include <list>
#include <vector>

using namespace std;

enum Position {
	FREE,
	MINE
};

struct BoardPos {
	Position state;
	int row;
	int col;
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
	list<BoardPos> revealPosition(int x, int y);
	bool allMinesRevealed();

	friend std::ostream& operator<<(std::ostream& os, const Board& obj);

private:
	struct BoardPosInternal {
		Position state{ FREE };
		bool revealed{ false };
	};
	void clearMines();

	std::vector<std::vector<BoardPosInternal> > mPos;
	int mRows, mCols, mTotalMines, mTotalMinesRevealed;
};
