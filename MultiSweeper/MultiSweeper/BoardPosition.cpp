#include "BoardPosition.h"

using namespace std;


BoardPosition::BoardPosition(int row, int col) : BoardPosition(row, col, false)
{
}

BoardPosition::BoardPosition(int row, int col, bool isMine) : 
	position(row, col), mine(isMine), countNeighbourMines(0), revealed(false)
{
}


BoardPosition::~BoardPosition()
{
}

void BoardPosition::setCountNeighbourMines(unsigned int const& countNeighbourMines) {
	this->countNeighbourMines = countNeighbourMines;
}

void BoardPosition::setMine() {
	this->mine = true;
}

void BoardPosition::setFree() {
	this->mine = false;
}

void BoardPosition::setRevealed() {
	revealed = true;
}

unsigned int const& BoardPosition::getCountNeighbourMines() const {
	return countNeighbourMines;
}

bool const& BoardPosition::isRevealed() const {
	return revealed;
}

bool const& BoardPosition::isMine() const {
	return mine;
}

std::tuple<int, int> const& BoardPosition::getPosition() const {
	return position;
}