/*! \brief CursesMenuExtended class represents an extension to CursesExtended
*          where it adds the possibility to represent menus and submenus.
*
*  Author: Diogo Silva <dbtdsilva@gmail.com>
*/

#include "CursesExtended.h"

#include <vector>
#include <functional>

class CursesMenuExtended : public CursesExtended
{
public:
	CursesMenuExtended();

protected:
	struct command
	{
		std::string text;
		std::function<void(WINDOW *)> function;
	};

	void display_menu(std::vector<struct command> options, std::string title);
	void process_menu_key(int key, std::vector<struct command> menu_vector);

private:
	int menu_old_opt_, menu_new_opt_;
};
