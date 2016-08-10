#include "SweeperConsole.h"

#include <iostream>

using namespace std;

SweeperConsole::SweeperConsole()
{
}


SweeperConsole::~SweeperConsole()
{
}


void SweeperConsole::gameStarted() {
	cout << "Game has started!" << endl;
}

void SweeperConsole::gameFinished() {
	cout << "Game has finished!" << endl;
}

void SweeperConsole::boardPosRevealed(int x, int y, Position state) {
	cout << state << " found" << endl;
}

void SweeperConsole::boardCreated(int height, int width) {
	cout << "Board created" << endl;
}

void SweeperConsole::playerWon(Player player) {
	cout << player.getUsername() << " has won" << endl;
}