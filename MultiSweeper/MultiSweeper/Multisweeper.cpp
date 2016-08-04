#include <iostream>
#include "Engine.h"

using namespace std;

int main(int argc, char **argv)
{
	Engine *oSweeper = new Engine();
	cout << *oSweeper << endl;


	cout << "Press any key to exit..." << flush;
	cin.get();

	delete oSweeper;
	return 0;
}