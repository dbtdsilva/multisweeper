#pragma once

#include <tuple>

class BoardPosition
{
public:
	BoardPosition(int row, int col);
	BoardPosition(int row, int col, bool isMine);
	~BoardPosition();

	void setCountNeighbourMines(unsigned int const& countNeighbourMines);
	void setMine();
	void setFree();
	void setRevealed(bool const& isRevealed);

	unsigned int const& getCountNeighbourMines() const;
	bool const& isRevealed() const;
	bool const& isMine() const;
	bool const& isFree() const;
	std::tuple<int, int> const& getPosition() const;

private:
	bool revealed;
	unsigned int countNeighbourMines;
	bool mine;
	std::tuple<int, int> position;
};

