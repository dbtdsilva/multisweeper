#include "Board.h"
#include <iostream>
#include <random>
#include <time.h>

using namespace std;

Board::Board(int rows, int cols, int totalMines)
{
	if (rows * cols <= totalMines) {
		throw runtime_error("Total number of mines should be less than the board size");
	}
	this->modifyBoard(rows, cols, totalMines);
	this->generateMines();
}

Board::~Board()
{
}

void Board::generateMines() {
	if (mTotalMines >= mRows * mCols) {
		return;
	}
	clearMines();

	srand((unsigned int) time(NULL));
	int minesPlaced = 0;
	int mineC, mineR;
	while (minesPlaced < mTotalMines) {
		mineC = rand() % mCols;
		mineR = rand() % mRows;

		if (mPos[mineR][mineC].state == FREE) {
			mPos[mineR][mineC].state = MINE;
			minesPlaced++;
		}
	}
	int x = 1;
}

void Board::modifyBoard(int rows, int cols, int totalMines) {
	this->modifyBoard(rows, cols);
	this->modifyNumberMines(totalMines);
}

void Board::modifyBoard(int rows, int cols) {
	this->mPos.resize(rows);
	for (int i = 0; i < rows; i++) {
		this->mPos[i].resize(cols);
	}
	this->mCols = cols;
	this->mRows = rows;
}

void Board::modifyNumberMines(int totalMines) {
	this->mTotalMines = totalMines;
}

list<BoardPos> Board::revealPosition(int row, int col) {
	if (mPos[row][col].revealed) {
		throw runtime_error("Position was already revealed");
	}
	list<BoardPos> positionsRevealed;
	positionsRevealed.push_back(BoardPos{ mPos[row][col].state, row, col });
	if (mPos[row][col].state == MINE) {
		mTotalMinesRevealed++;
	} else {

	}
	mPos[row][col].revealed = true;
	return positionsRevealed;
}

void Board::clearMines() {
	for (int h = 0; h < mRows; h++) {
		for (int w = 0; w < mCols; w++) {
			mPos[h][w].state = FREE;
			mPos[h][w].revealed = false;
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
			os << obj.mPos[i][j].state;
		}
		os << endl;
	}
	return os;
}