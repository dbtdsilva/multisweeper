#include "Engine.h"

#include <random>
#include <time.h>
#include <algorithm>

using namespace std;

int Engine::MAX_PLAYERS = 4;

Engine::Engine()
{
	this->currentStatus = START;
	this->currentPlayerIdx = 0;
	this->currentPlayer = nullptr;
	this->mBoard = unique_ptr<Board>(new Board(10, 5, 1000));
}

Engine::~Engine()
{
}

void Engine::startGame() {
	if (currentStatus != START)
		throw runtime_error("Game has already started");
	this->currentStatus = RUN;
	random_shuffle(mPlayers.begin(), mPlayers.end());
}

void Engine::joinGame(string username) {
	if (currentStatus != START)
		throw runtime_error("Game already started, players won't be able to join now");
	Player p(username);
	mPlayers.push_back(p);
}

void Engine::turnPlayed(int x, int y)
{
	if (currentStatus != RUN)
		throw runtime_error("Game isn't running");

	Position revealed = mBoard->revealPosition(x, y);
	if (revealed == FREE) 
		nextPlayer();

	if (isGameFinished()) {
		currentStatus = FINISH;
		currentPlayer = nullptr;
	}
}

void Engine::surrender(Player player)
{
}

Player* Engine::nextPlayer() {
	if (currentPlayer == nullptr) {
		srand(time(NULL));
		currentPlayerIdx = rand() % mPlayers.size();
		currentPlayer = &mPlayers[currentPlayerIdx];
		return currentPlayer;
	}
	currentPlayer = &mPlayers[++currentPlayerIdx % mPlayers.size()];
	return currentPlayer;
}

Player* Engine::getCurrentPlayer() {
	return currentPlayer;
}

bool Engine::isGameFinished() {
	return mBoard->allMinesRevealed();
}

ostream& operator<<(ostream& os, const Engine& obj) {
	os << *(obj.mBoard);
	return os;
}