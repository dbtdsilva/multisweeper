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
	Board(int width, int height, int totalMines);
	virtual ~Board();

	void generateMines();
	void modifyBoard(int width, int height, int totalMines);
	Position revealPosition(int x, int y);
	bool allMinesRevealed();

	friend std::ostream& operator<<(std::ostream& os, const Board& obj);

private:
	void clearMines();

	std::vector<std::vector<BoardPos> > mPos;
	int mHeight;
	int mWidth;
	int mTotalMines;
	int mTotalMinesRevealed;
};
