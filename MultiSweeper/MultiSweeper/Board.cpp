#include "Board.h"
#include <iostream>
#include <random>
#include <time.h>

#include "SweeperException.h"

using namespace std;

Board::Board(int rows, int cols, int total_mines) :
	rows_(rows), cols_(cols), total_mines_(total_mines), total_mines_revealed_(0)
{
	this->modify_board(rows, cols, total_mines);
}

Board::~Board()
{
}

void Board::generate_mines() 
{
	if (total_mines_ >= rows_ * cols_) {
		throw SweeperException(SweeperError::MINES_EXCEED);
	}
	clear_mines();

	srand((unsigned int) time(NULL));
	int minesPlaced = 0;
	int mineC, mineR;
	while (minesPlaced < total_mines_) {
		mineC = rand() % cols_;
		mineR = rand() % rows_;

		if (!positions_[mineR][mineC]->is_mine()) {
			positions_[mineR][mineC]->set_mine();
			minesPlaced++;
		}
	}

	int positionMineCounter;
	// Run over all board
	for (int row = 0; row < rows_; row++) {
		for (int col = 0; col < cols_; col++) {
			positionMineCounter = 0;
			// Count all mines in adjacencies and itself
			for (int adjRow = -1; adjRow <= 1; adjRow++) {
				for (int adjCol = -1; adjCol <= 1; adjCol++) {
					if (row + adjRow >= 0 && row + adjRow < rows_ 
							&& col + adjCol >= 0 && col + adjCol < cols_ 
							&& positions_[row + adjRow][col + adjCol]->is_mine()) {
						positionMineCounter++;
					}
				}
			}
			positions_[row][col]->set_count_neighbour_mines(positionMineCounter);
		}
	}
}

void Board::modify_board(int rows, int cols, int total_mines) 
{
	this->total_mines_ = total_mines;
	
	this->positions_.resize(rows);
	for (int i = 0; i < rows; i++) {
		this->positions_[i].resize(cols);
		for (int j = 0; j < cols; j++) {
			this->positions_[i][j] = make_unique<BoardPosition>(i, j);
		}
	}
	this->cols_ = cols;
	this->rows_ = rows;

	this->generate_mines();
}

list<BoardPosition *> Board::reveal_position(int row, int col) 
{
	list<BoardPosition *> positionsRevealed;
	// If the position was already revealed, return empty.
	if (positions_[row][col]->is_revealed()) {
		throw SweeperException(SweeperError::POSITION_ALREADY_REVEALED);
	}
	positionsRevealed.push_back(positions_[row][col].get());
	if (positions_[row][col]->is_mine()) {
		total_mines_revealed_++;
	} else {
		for (BoardPosition * freePosition : reveal_free_positions(row, col))
			positionsRevealed.push_back(freePosition);
	}
	positions_[row][col]->set_revealed();
	return positionsRevealed;
}

list<BoardPosition *> Board::reveal_free_positions(int row, int col) 
{
	list<BoardPosition *> positionsRevealed;
	positions_[row][col]->set_revealed();
	if (positions_[row][col]->get_count_neighbour_mines() != 0)
		return positionsRevealed;

	for (int adjRow = -1; adjRow <= 1; adjRow++) {
		for (int adjCol = -1; adjCol <= 1; adjCol++) {
			if ((adjRow != 0 || adjCol != 0) 
					&& row + adjRow >= 0 && row + adjRow < rows_
					&& col + adjCol >= 0 && col + adjCol < cols_
					&& !positions_[row + adjRow][col + adjCol]->is_revealed())
			{
				positionsRevealed.push_back(positions_[row + adjRow][col + adjCol].get());
				for (BoardPosition * freePosition : reveal_free_positions(row + adjRow, col + adjCol))
					positionsRevealed.push_back(freePosition);
			}
		}
	}
	return positionsRevealed;
}

void Board::clear_mines() 
{
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			positions_[i][j] = make_unique<BoardPosition>(i, j);
		}
	}
	this->total_mines_revealed_ = 0;
}

bool Board::all_mines_revealed() {
	return total_mines_revealed_ == total_mines_;
}

ostream& operator<<(ostream& os, const Board& obj) 
{
	for (int i = 0; i < obj.rows_; i++) {
		for (int j = 0; j < obj.cols_; j++) {
			os << obj.positions_[i][j]->is_mine();
		}
		os << endl;
	}
	return os;
}