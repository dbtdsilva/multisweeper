/*! \brief BoardPosition class which represents a pair of row, pair and each pair 
*          has a mine associated to it.
*
*  This class represents a position, where each position has several values associated
*  to it, such as, neighbour mines to that position, also says when position is a mine 
*  or not. Also, shows when the position is hidden or revealed.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#include "BoardPosition.h"

using namespace std;


BoardPosition::BoardPosition(int row, int col) : BoardPosition(row, col, false)
{
}

BoardPosition::BoardPosition(int row, int col, bool isMine) : 
	position_(row, col), mine_(isMine), count_neighbour_mines_(0), revealed_(false)
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