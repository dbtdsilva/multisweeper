#pragma once

#include <memory>
#include <list>
#include <vector>

#include "BoardPosition.h"

class Board
{
public:	
	Board(int rows, int cols, int total_mines);
	Board(const Board & other);
	Board(Board&& other);
	virtual ~Board();

	Board& operator=(const Board& other);
	Board& operator=(Board&& other);

	void generate_mines();
	void modify_board(int rows, int cols, int total_mines);
	std::list<BoardPosition *> reveal_position(int row, int col, bool special_bomb = false);
	bool all_mines_revealed();
	int const& get_rows() const;
	int const& get_total_mines() const;
	int const& get_cols() const;
	int const& get_total_mines_revealed() const;
	friend std::ostream& operator<<(std::ostream& os, const Board& obj);

private:
	std::list<BoardPosition *> reveal_free_positions(int row, int col);
	void clear_mines();

	std::vector<std::vector<std::unique_ptr<BoardPosition> > > positions_;
	int rows_, cols_, total_mines_, total_mines_revealed_;
};
