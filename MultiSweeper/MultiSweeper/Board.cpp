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

Board::~Board() {

}

Board::Board(const Board& other)
{
	*this = other;
}

Board::Board(Board&& other) : rows_(other.rows_), cols_(other.cols_),
	total_mines_(other.total_mines_), total_mines_revealed_(other.total_mines_revealed_)
{
	for (int i = 0; i < positions_.size(); i++) {
		for (int j = 0; j < positions_[j].size(); j++) {
			this->positions_[i][j] = std::move(other.positions_[i][j]);
		}
	}
}

Board& Board::operator=(const Board& other)
{
	if (this != &other) 
	{
		this->rows_ = other.rows_;
		this->cols_ = other.cols_;
		this->total_mines_ = other.total_mines_;
		this->total_mines_revealed_ = other.total_mines_revealed_;
		for (int i = 0; i < positions_.size(); i++) {
			for (int j = 0; j < positions_[j].size(); j++) {
				this->positions_[i][j] = make_unique<BoardPosition>(*(other.positions_[i][j]));
			}
		}
	}
	return *this;
}

Board& Board::operator=(Board&& other)
{
	if (this != &other) 
	{
		this->rows_ = other.rows_;
		this->cols_ = other.cols_;
		this->total_mines_ = other.total_mines_;
		this->total_mines_revealed_ = other.total_mines_revealed_;
		for (int i = 0; i < positions_.size(); i++) {
			for (int j = 0; j < positions_[j].size(); j++) {
				this->positions_[i][j] = std::move(other.positions_[i][j]);
			}
		}
	}
	return *this;
}

void Board::generate_mines() 
{
	if (total_mines_ >= rows_ * cols_) {
		throw SweeperException(SweeperError::MINES_EXCEED);
	}
	clear_mines();

	srand((unsigned int) time(NULL));
	int mines_places = 0;
	int mine_col, mine_row;
	while (mines_places < total_mines_) {
		mine_col = rand() % cols_;
		mine_row = rand() % rows_;

		if (!positions_[mine_row][mine_col]->is_mine()) {
			positions_[mine_row][mine_col]->set_mine();
			mines_places++;
		}
	}

	int position_mine_counter;
	// Run over all board
	for (int row = 0; row < rows_; row++) {
		for (int col = 0; col < cols_; col++) {
			position_mine_counter = 0;
			// Count all mines in adjacencies and itself
			for (int adj_row = -1; adj_row <= 1; adj_row++) {
				for (int adj_col = -1; adj_col <= 1; adj_col++) {
					if (row + adj_row >= 0 && row + adj_row < rows_
							&& col + adj_col >= 0 && col + adj_col < cols_
							&& positions_[row + adj_row][col + adj_col]->is_mine()) {
						position_mine_counter++;
					}
				}
			}
			positions_[row][col]->set_count_neighbour_mines(position_mine_counter);
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
	list<BoardPosition *> positions_revealed;
	// If the position was already revealed, return empty.
	if (positions_[row][col]->is_revealed()) {
		throw SweeperException(SweeperError::POSITION_ALREADY_REVEALED);
	}
	positions_[row][col]->set_revealed();
	positions_revealed.push_back(positions_[row][col].get());
	if (positions_[row][col]->is_mine()) {
		total_mines_revealed_++;
	} else if (positions_[row][col]->get_count_neighbour_mines() == 0) {
		for (BoardPosition * new_position_revealed : reveal_free_positions(row, col))
			positions_revealed.push_back(new_position_revealed);
	}
	return positions_revealed;
}

list<BoardPosition *> Board::reveal_free_positions(int row, int col) 
{
	list<BoardPosition *> positions_revealed;
	vector<BoardPosition *> empty_neighbour_mines = { positions_[row][col].get() };
	BoardPosition * zero_position;
	while (!empty_neighbour_mines.empty()) {
		zero_position = empty_neighbour_mines.back();
		tuple<int, int> const& value = zero_position->get_position();
		int const& zero_row = get<0>(value);
		int const& zero_col = get<1>(value);
		empty_neighbour_mines.pop_back();
		for (int adj_row = -1; adj_row <= 1; adj_row++) {
			for (int adj_col = -1; adj_col <= 1; adj_col++) {
				int new_row = zero_row + adj_row;
				int new_col = zero_col + adj_col;
				if (new_row >= 0 && new_row < rows_
					&& new_col >= 0 && new_col < cols_
					&& !positions_[new_row][new_col]->is_revealed())
				{
					positions_revealed.push_back(positions_[new_row][new_col].get());
					positions_[new_row][new_col]->set_revealed();
					if (positions_[new_row][new_col]->get_count_neighbour_mines() == 0) {
						empty_neighbour_mines.push_back(positions_[new_row][new_col].get());
					}
				}
			}
		}
	}
	return positions_revealed;
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

int const& Board::get_rows() const
{
	return rows_;
}

int const& Board::get_total_mines() const
{
	return total_mines_;
}

int const& Board::get_cols() const
{
	return cols_;
}

int const& Board::get_total_mines_revealed() const
{
	return total_mines_revealed_;
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