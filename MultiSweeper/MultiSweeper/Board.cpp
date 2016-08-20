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

		if (!mPos[mineR][mineC]->isMine()) {
			mPos[mineR][mineC]->setMine();
			minesPlaced++;
		}
	}

	int positionMineCounter;
	// Run over all board
	for (int row = 0; row < mRows; row++) {
		positionMineCounter = 0;
		for (int col = 0; col < mCols; col++) {
			// Count all mines in adjacencies and itself
			for (int adjRow = -1; adjRow <= 1; adjRow++) {
				for (int adjCol = -1; adjCol <= 1; adjCol++) {
					if (row + adjRow >= 0 && row + adjRow < mRows 
							&& col + adjCol >= 0 && col + adjCol < mCols 
							&& mPos[row + adjRow][col + adjCol]->isMine()) {
						positionMineCounter++;
					}
				}
			}
			mPos[row][col]->setCountNeighbourMines(positionMineCounter);
		}
	}
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
	list<BoardPosition *> positionsRevealed;
	// If the position was already revealed, return empty.
	if (mPos[row][col]->isRevealed()) {
		throw runtime_error("Position was already revealed");
	}
	positionsRevealed.push_back(mPos[row][col].get());
	if (mPos[row][col]->isMine()) {
		mTotalMinesRevealed++;
	} else {
		for (BoardPosition * freePosition : revealFreePositions(row, col))
			positionsRevealed.push_back(freePosition);
	}
	mPos[row][col]->setRevealed();
	return positionsRevealed;
}

list<BoardPosition *> Board::revealFreePositions(int row, int col) {
	list<BoardPosition *> positionsRevealed;
	for (int adjRow = -1; adjRow <= 1; adjRow++) {
		for (int adjCol = -1; adjCol <= 1; adjCol++) {
			if ((adjRow != 0 || adjCol != 0) 
					&& row + adjRow >= 0 && row + adjRow < mRows
					&& col + adjCol >= 0 && col + adjCol < mCols
					&& !mPos[row + adjRow][col + adjCol]->isMine()) {
				mPos[row + adjRow][col + adjCol]->setRevealed();
				positionsRevealed.push_back(mPos[row + adjRow][col + adjCol].get());
				if (mPos[row + adjRow][col + adjCol]->getCountNeighbourMines() == 0 &&
					!mPos[row + adjRow][col + adjCol]->isRevealed())
				{
					for (BoardPosition * freePosition : revealFreePositions(row + adjRow, col + adjCol))
						positionsRevealed.push_back(freePosition);
				}
			}
		}
	}
	return positionsRevealed;
}

void Board::clearMines() {
	for (int i = 0; i < mRows; i++) {
		for (int j = 0; j < mCols; j++) {
			mPos[i][j] = unique_ptr<BoardPosition>(new BoardPosition(i, j));
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