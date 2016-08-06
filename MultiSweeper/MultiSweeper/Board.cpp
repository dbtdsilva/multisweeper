#include "Board.h"
#include <iostream>
#include <random>
#include <time.h>

using namespace std;

Board::Board(int width, int height, int totalMines)
{
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

		if (mPos[mineH][mineW] == FREE) {
			mPos[mineH][mineW] = MINE;
			minesPlaced++;
		}
	}
}

void Board::modifyBoard(int width, int height, int totalMines) {
	this->mPos.resize(height);
	for (int i = 0; i < height; i++) {
		this->mPos[i].resize(width, FREE);
	}
	this->mTotalMines = totalMines;
	this->mWidth = width;
	this->mHeight = height;
}

void Board::clearMines() {
	for (int h = 0; h < mHeight; h++) {
		for (int w = 0; w < mWidth; w++) {
			mPos[h][w] = FREE;
		}
	}
}

ostream& operator<<(ostream& os, const Board& obj) {
	for (int i = 0; i < obj.mHeight; i++) {
		for (int j = 0; j < obj.mWidth; j++) {
			os << obj.mPos[i][j];
		}
		os << endl;
	}
	return os;
}