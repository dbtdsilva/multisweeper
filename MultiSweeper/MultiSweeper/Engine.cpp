#include "Engine.h"

#include <string>
#include <random>
#include <time.h>
#include <list>
#include <algorithm>
#include "SweeperException.h"
#include "SweeperError.h"

using namespace std;

Engine::Engine() : Engine(nullptr)
{
}

Engine::Engine(InterfaceVisual* iv) : Engine(iv, 10, 5, 10)
{
}

Engine::Engine(InterfaceVisual* iv, int nRows, int nCols, int nMines) :
	kMaxPlayers(4), currentStatus(START), currentPlayerIdx(0), currentPlayer(nullptr),
	mBoard(make_unique<Board>(nRows, nCols, nMines)), mInteraction(iv)
{
}

Engine::~Engine()
{
}

void Engine::start_game() {
	if (!verify_game_status(START)) return;
	if (this->mPlayers.size() == 0) {
		mInteraction->dispatch_error(SweeperError::NO_PLAYERS);
		return;
	}

	this->currentStatus = RUN;
	random_shuffle(mPlayers.begin(), mPlayers.end());
	next_player();

	this->mInteraction->game_started();
}

void Engine::join_game(string username) {
	if (!verify_game_status(START)) return;
	Player p(username);
	mPlayers.push_back(p);
}

void Engine::leave_game(string username) {
	if (!verify_game_status(START)) return;
	auto it = std::find(mPlayers.begin(), mPlayers.end(), username);
	if (it != mPlayers.end())
		mPlayers.erase(it);
}

void Engine::leave_game(int id) {
	if (!verify_game_status(START)) return;
	mPlayers.erase(mPlayers.begin() + id);
}

void Engine::modify_board(int nRows, int nCols, int nTotalMines) {
	if (!verify_game_status(START)) return;

	try {
		mBoard->modify_board(nRows, nCols, nTotalMines);
	} catch (SweeperException& ex) {
		mInteraction->dispatch_error(ex.get_sweeper_error());
	}
}

void Engine::turn_played(int row, int col) {
	if (!verify_game_status(RUN)) return;

	list<BoardPosition *> listRevealed;
	try {
		listRevealed = mBoard->revealPosition(row, col);
	} catch (SweeperException& ex) {
		mInteraction->dispatch_error(ex.get_sweeper_error());
		return;
	}

	bool foundMine = false;
	for (BoardPosition * revealed : listRevealed) {
		if (revealed->isMine()) {
			foundMine = true;
			break;
		}
	}

	if (!foundMine) 
		next_player();
	this->mInteraction->board_position_revealed(listRevealed);

	if (is_game_finished()) {
		currentStatus = START;
		currentPlayer = nullptr;
		this->mInteraction->game_finished();
	}
}

void Engine::surrender(Player player)
{
}

Player* Engine::next_player() {
	if (!verify_game_status(RUN)) return nullptr;

	if (currentPlayer == nullptr) {
		srand((unsigned int) time(NULL));
		currentPlayerIdx = rand() % mPlayers.size();
		currentPlayer = &mPlayers[currentPlayerIdx];
		return currentPlayer;
	}
	currentPlayer = &mPlayers[++currentPlayerIdx % mPlayers.size()];
	return currentPlayer;
}

Player const& Engine::get_current_player() const {
	return *currentPlayer;
}

std::vector<Player> const& Engine::get_players_list() const {
	return mPlayers;
}

ostream& operator<<(ostream& os, const Engine& obj) {
	os << *(obj.mBoard);
	return os;
}

bool Engine::is_game_finished() {
	return mBoard->allMinesRevealed();
}

bool Engine::verify_game_status(Status expected) {
	if (expected == currentStatus)
		return true;

	if (currentStatus == START)
		mInteraction->dispatch_error(SweeperError::GAME_NOT_RUNNING);
	if (currentStatus == RUN)
		mInteraction->dispatch_error(SweeperError::GAME_ALREADY_STARTED);
	return false;
}