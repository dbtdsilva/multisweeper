#include "Engine.h"

#include <string>
#include <random>
#include <time.h>
#include <list>
#include <algorithm>
#include "SweeperException.h"
#include "SweeperError.h"

using namespace std;

int Engine::MAX_PLAYERS = 4;

Engine::Engine() : Engine(nullptr)
{
}

Engine::Engine(InterfaceVisual* iv) : Engine(iv, 10, 5, 10)
{
}

Engine::Engine(InterfaceVisual* iv, int nRows, int nCols, int nMines) 
{
	this->currentStatus = START;
	this->currentPlayerIdx = 0;
	this->currentPlayer = nullptr;
	this->mBoard =  make_unique<Board>(nRows, nCols, nMines);
	this->mInteraction = iv;
}

Engine::~Engine()
{
}

void Engine::startGame() {
	assertGameStatus(START);
	if (this->mPlayers.size() == 0)
		throw SweeperException(SweeperError::NO_PLAYERS);
	this->currentStatus = RUN;
	random_shuffle(mPlayers.begin(), mPlayers.end());
	nextPlayer();

	this->mInteraction->gameStarted();
}

void Engine::joinGame(string username) {
	assertGameStatus(START);
	Player p(username);
	mPlayers.push_back(p);
}

void Engine::leaveGame(string username) {
	assertGameStatus(START);
	auto it = std::find(mPlayers.begin(), mPlayers.end(), username);
	if (it != mPlayers.end())
		mPlayers.erase(it);
}

void Engine::leaveGame(int id) {
	assertGameStatus(START);
	mPlayers.erase(mPlayers.begin() + id);
}

void Engine::modifyBoard(int nRows, int nCols, int nTotalMines) {
	assertGameStatus(START);
	mBoard->modifyBoard(nRows, nCols, nTotalMines);
}

void Engine::turnPlayed(int row, int col) {
	assertGameStatus(RUN);

	list<BoardPosition *> listRevealed = mBoard->revealPosition(row, col);
	bool foundMine = false;
	for (BoardPosition * revealed : listRevealed) {
		if (revealed->isMine()) {
			foundMine = true;
			break;
		}
	}

	if (!foundMine) 
		nextPlayer();
	this->mInteraction->boardPosRevealed(listRevealed);

	if (isGameFinished()) {
		currentStatus = START;
		currentPlayer = nullptr;
		this->mInteraction->gameFinished();
	}
}

void Engine::surrender(Player player)
{
}

Player* Engine::nextPlayer() {
	assertGameStatus(RUN);

	if (currentPlayer == nullptr) {
		srand((unsigned int) time(NULL));
		currentPlayerIdx = rand() % mPlayers.size();
		currentPlayer = &mPlayers[currentPlayerIdx];
		return currentPlayer;
	}
	currentPlayer = &mPlayers[++currentPlayerIdx % mPlayers.size()];
	return currentPlayer;
}

Player const& Engine::getCurrentPlayer() const {
	return *currentPlayer;
}

std::vector<Player> const& Engine::getPlayersList() const {
	return mPlayers;
}

ostream& operator<<(ostream& os, const Engine& obj) {
	os << *(obj.mBoard);
	return os;
}

bool Engine::isGameFinished() {
	return mBoard->allMinesRevealed();
}

void Engine::assertGameStatus(Status expected) {
	if (expected == currentStatus)
		return;

	if (currentStatus == START)
		throw SweeperException(SweeperError::GAME_NOT_RUNNING);
	if (currentStatus == RUN)
		throw SweeperException(SweeperError::GAME_ALREADY_STARTED);
}