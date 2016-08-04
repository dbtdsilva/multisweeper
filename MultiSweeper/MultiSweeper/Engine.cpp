#include "Engine.h"

using namespace std;

int Engine::MAX_PLAYERS = 4;

Engine::Engine()
{
	this->currentStatus = START;
	this->currentNumPlayers = 0;

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

void Engine::getStatus() const {
}

ostream& operator<<(ostream& os, const Engine& obj) {
	os << *(obj.mBoard);
	return os;
}