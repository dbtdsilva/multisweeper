#include "Board.h"
#include <iostream>
#include <random>
#include <time.h>

using namespace std;

Board::Board(int width, int height, int totalMines)
{
	if (width * height <= totalMines) {
		throw runtime_error("Total number of mines should be less than the board size");
	}
	this->modifyBoard(width, height, totalMines);
	this->generateMines();
}

Board::~Board()
{
}

void Board::generateMines() {
	if (mTotalMines >= mWidth * mHeight) {
		return;
	}
	clearMines();

	srand(time(NULL));
	int minesPlaced = 0;
	int mineW, mineH;
	while (minesPlaced < mTotalMines) {
		mineW = rand() % mWidth;
		mineH = rand() % mHeight;

		if (mPos[mineH][mineW].state == FREE) {
			mPos[mineH][mineW].state = MINE;
			minesPlaced++;
		}
	}
	int x = 1;
}

void Board::modifyBoard(int width, int height, int totalMines) {
	this->mPos.resize(height);
	for (int i = 0; i < height; i++) {
		this->mPos[i].resize(width);
	}
	this->mTotalMines = totalMines;
	this->mWidth = width;
	this->mHeight = height;
}

Position Board::revealPosition(int x, int y) {
	if (mPos[y][x].revealed) {
		throw runtime_error("Position was already revealed");
	}

	if (mPos[y][x].state == MINE)
		mTotalMinesRevealed++;
	mPos[y][x].revealed = true;
	return mPos[y][x].state;
}

void Board::clearMines() {
	for (int h = 0; h < mHeight; h++) {
		for (int w = 0; w < mWidth; w++) {
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
	for (int i = 0; i < obj.mHeight; i++) {
		for (int j = 0; j < obj.mWidth; j++) {
			os << obj.mPos[i][j].state;
		}
		os << endl;
	}
	return os;
}