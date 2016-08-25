#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <curses.h>

class CursesExtended
{
public:
	CursesExtended();
	virtual ~CursesExtended();
	virtual void loop() = 0;

protected:
	void init();
	template <typename T>
	void mvscanw_robust(std::string intro, int start_row, T * return_value);
	void mvaddstr_centered(int row, std::string message);
	void display_error(int row, std::string message);
	void display_border();
	void clear_specific(int row, int size);
	void change_color_schema(bool direction = false);
	const int& get_color_schema_index();
	const int& get_height();
	const int& get_width();

	WINDOW * window_;
	std::vector<std::tuple<int, int> > pair_colors_;

private:
	int color_schema_index_;
	int height_, width_;
};

template<typename T>
inline void CursesExtended::mvscanw_robust(std::string intro, int start_row, T * return_value)
{
	attrset(A_BOLD);
	mvaddstr_centered(start_row, intro);
	attrset(A_NORMAL);
	refresh();
	echo();
	curs_set(true);

	int valid_scan_result;
	if (is_same<T, int>::value) {
		valid_scan_result = mvscanw(start_row + 1, (COLS - 10) / 2, "%d", return_value);
	}
	else if (is_same<T, string>::value) {
		char word[31];
		valid_scan_result = mvscanw(start_row + 1, (COLS - (int)intro.size()) / 2, "%30s", word);
		stringstream ss(word);
		ss >> *return_value;
	}

	if (!valid_scan_result) {
		throw runtime_error("Invalid input received");
	}

	noecho();
	curs_set(false);
	attrset(A_BOLD);
	attron(COLOR_PAIR(color_schema_index_));
	mvaddstr_centered(start_row + 3, "Press any key to continue");
	attroff(COLOR_PAIR(color_schema_index_));
	attrset(A_NORMAL);
	
}
