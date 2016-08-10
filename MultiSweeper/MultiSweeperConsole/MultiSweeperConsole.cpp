#include <iostream>

#include "SweeperConsole.h"
#include "Engine.h"

using namespace std;

int main(int argc, char **argv)
{
	unique_ptr<SweeperConsole> pSweeperConsole = unique_ptr<SweeperConsole>(new SweeperConsole());
	unique_ptr<Engine> pEngine = unique_ptr<Engine>(new Engine(pSweeperConsole.get()));


	cout << "Press any key to exit..." << flush;
	cin.get();
	return 0;
}