#pragma once

#include <tuple>

class BoardPosition
{
public:
	BoardPosition(int row, int col);
	BoardPosition(int row, int col, bool is_mine);
	~BoardPosition();

	void set_count_neighbour_mines(unsigned int const& count_neighbour_mines);
	void set_mine();
	void set_free();
	void set_revealed();

	unsigned int const& get_count_neighbour_mines() const;
	bool const& is_revealed() const;
	bool const& is_mine() const;
	std::tuple<int, int> const& get_position() const;

private:
	bool revealed_;
	unsigned int count_neighbour_mines_;
	bool mine_;
	std::tuple<int, int> position_;
};

