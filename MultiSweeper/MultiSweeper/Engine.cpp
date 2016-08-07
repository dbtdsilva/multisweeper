#include "Engine.h"

using namespace std;

int Engine::MAX_PLAYERS = 4;

Engine::Engine()
{
	this->currentStatus = START;

	this->mBoard = unique_ptr<Board>(new Board(10, 5, 10));
}

Engine::~Engine()
{
}

void Engine::startGame() {
	this->currentStatus = RUN;
}

void Engine::joinGame(string username) {

}

void Engine::turnPlayed(int x, int y)
{
	Position revealed = mBoard->revealPosition(x, y);
	if (revealed == MINE) {
		// Next player
	}
}

void Engine::surrender(Player player)
{
}

ostream& operator<<(ostream& os, const Engine& obj) {
	os << *(obj.mBoard);
	return os;
}