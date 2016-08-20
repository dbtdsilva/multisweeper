#include "Board.h"
#include <iostream>
#include <random>
#include <time.h>

using namespace std;

Board::Board(int rows, int cols, int totalMines)
{
	this->modifyBoard(rows, cols, totalMines);
}

Board::~Board()
{
}

void Board::generateMines() {
	if (mTotalMines >= mRows * mCols) {
		throw runtime_error("Total number of mines should be less than the board size");
	}
	clearMines();

	srand((unsigned int) time(NULL));
	int minesPlaced = 0;
	int mineC, mineR;
	while (minesPlaced < mTotalMines) {
		mineC = rand() % mCols;
		mineR = rand() % mRows;

		if (mPos[mineR][mineC]->isFree()) {
			mPos[mineR][mineC]->setMine();
			minesPlaced++;
		}
	}
	int x = 1;
}

void Board::modifyBoard(int rows, int cols, int totalMines) {
	this->mTotalMines = totalMines;
	
	this->mPos.resize(rows);
	for (int i = 0; i < rows; i++) {
		this->mPos[i].resize(cols);
		for (int j = 0; j < cols; j++) {
			this->mPos[i][j] = unique_ptr<BoardPosition>(new BoardPosition(i, j));
		}
	}
	this->mCols = cols;
	this->mRows = rows;

	this->generateMines();
}

list<BoardPosition *> Board::revealPosition(int row, int col) {
	if (mPos[row][col]->isRevealed()) {
		throw runtime_error("Position was already revealed");
	}
	list<BoardPosition *> positionsRevealed;
	positionsRevealed.push_back(mPos[row][col].get());
	if (mPos[row][col]->isMine()) {
		mTotalMinesRevealed++;
	} else {

	}
	mPos[row][col]->setRevealed(true);
	return positionsRevealed;
}

void Board::clearMines() {
	for (int h = 0; h < mRows; h++) {
		for (int w = 0; w < mCols; w++) {
			mPos[h][w]->setFree();
			mPos[h][w]->setRevealed(false);
		}
	}
	this->mTotalMinesRevealed = 0;
}

bool Board::allMinesRevealed() {
	return mTotalMinesRevealed == mTotalMines;
}

ostream& operator<<(ostream& os, const Board& obj) {
	for (int i = 0; i < obj.mRows; i++) {
		for (int j = 0; j < obj.mCols; j++) {
			os << obj.mPos[i][j]->isMine();
		}
		os << endl;
	}
	return os;
}