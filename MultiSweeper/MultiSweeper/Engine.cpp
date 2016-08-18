#include "Engine.h"

#include <string>
#include <random>
#include <time.h>
#include <algorithm>

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
	this->mBoard = unique_ptr<Board>(new Board(nRows, nCols, nMines));
	this->mInteraction = iv;
}

Engine::~Engine()
{
}

void Engine::startGame() {
	if (currentStatus == RUN)
		throw runtime_error("Game has already started");
	this->currentStatus = RUN;
	random_shuffle(mPlayers.begin(), mPlayers.end());

	this->mInteraction->gameStarted();
}

void Engine::joinGame(string username) {
	if (currentStatus == RUN)
		throw runtime_error("Game already started, players won't be able to join now");
	Player p(username);
	mPlayers.push_back(p);
}

void Engine::leaveGame(string username) {
	auto it = std::find(mPlayers.begin(), mPlayers.end(), username);
	if (it != mPlayers.end())
		mPlayers.erase(it);
}

void Engine::leaveGame(int id) {
	mPlayers.erase(mPlayers.begin() + id);
}

void Engine::modifyBoard(int nRows, int nCols) {
	mBoard->modifyBoard(nRows, nCols);
}

void Engine::modifyNumberMines(int nMines) {
	mBoard->modifyNumberMines(nMines);
}

void Engine::turnPlayed(int x, int y)
{
	if (currentStatus != RUN)
		throw runtime_error("Game isn't running");

	Position revealed = mBoard->revealPosition(x, y);
	if (revealed == FREE) 
		nextPlayer();
	this->mInteraction->boardPosRevealed(x, y, revealed);

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

bool Engine::isGameFinished() {
	return mBoard->allMinesRevealed();
}

ostream& operator<<(ostream& os, const Engine& obj) {
	os << *(obj.mBoard);
	return os;
}