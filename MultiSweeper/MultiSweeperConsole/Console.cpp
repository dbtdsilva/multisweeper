#include "Console.h"

#include <iostream>

using namespace std;

Console::Console()
{
}


Console::~Console()
{
}


void Console::gameStarted() {
	cout << "Game has started!" << endl;
}

void Console::gameFinished() {
	cout << "Game has finished!" << endl;
}

void Console::boardPosRevealed(int x, int y, Position state) {
	cout << state << " found" << endl;
}

void Console::boardCreated(int height, int width) {
	cout << "Board created" << endl;
}

void Console::playerWon(Player player) {
	cout << player.getUsername() << " has won" << endl;
}