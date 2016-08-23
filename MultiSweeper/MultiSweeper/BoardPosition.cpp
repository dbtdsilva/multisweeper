#include "BoardPosition.h"

using namespace std;


BoardPosition::BoardPosition(int row, int col) : BoardPosition(row, col, false)
{
}

BoardPosition::BoardPosition(int row, int col, bool isMine) : 
	position_(row, col), mine_(isMine), count_neighbour_mines_(0), revealed_(false)
{
}


BoardPosition::~BoardPosition()
{
}

void BoardPosition::set_count_neighbour_mines(unsigned int const& count_neighbour_mines) 
{
	this->count_neighbour_mines_ = count_neighbour_mines;
}

void BoardPosition::set_mine() 
{
	this->mine_ = true;
}

void BoardPosition::set_free() 
{
	this->mine_ = false;
}

void BoardPosition::set_revealed() 
{
	revealed_ = true;
}

unsigned int const& BoardPosition::get_count_neighbour_mines() const 
{
	return count_neighbour_mines_;
}

bool const& BoardPosition::is_revealed() const 
{
	return revealed_;
}

bool const& BoardPosition::is_mine() const 
{
	return mine_;
}

std::tuple<int, int> const& BoardPosition::get_position() const 
{
	return position_;
}