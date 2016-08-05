#include "Board.h"
#include <iostream>

using namespace std;

Board::Board(int width, int height, int nmines)
{
	mPos.resize(height);
	for (int i = 0; i < height; i++) {
		mPos[i].resize(width, FREE);
	}
	mTotalMines = nmines;
	mWidth = width;
	mHeight = height;
}

Board::~Board()
{
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