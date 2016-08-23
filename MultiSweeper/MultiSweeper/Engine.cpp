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

Engine::Engine(InterfaceVisual* interaction) : Engine(interaction, 10, 5, 10)
{
}

Engine::Engine(InterfaceVisual* interaction, int rows, int cols, int mines) :
	kMaxPlayers(4), current_status_(START), current_player_index_(0), current_player_(nullptr),
	board_(make_unique<Board>(rows, cols, mines)), interaction_(interaction)
{
}

Engine::~Engine()
{
}

void Engine::start_game() 
{
	if (!verify_game_status(START)) return;
	if (this->players_.size() == 0) {
		interaction_->dispatch_error(SweeperError::NO_PLAYERS);
		return;
	}

	this->current_status_ = RUN;
	random_shuffle(players_.begin(), players_.end());
	next_player();

	this->interaction_->game_started();
}

void Engine::join_game(string username) 
{
	if (!verify_game_status(START)) return;
	Player p(username);
	players_.push_back(p);
}

void Engine::leave_game(string username) 
{
	if (!verify_game_status(START)) return;
	auto it = std::find(players_.begin(), players_.end(), username);
	if (it != players_.end())
		players_.erase(it);
}

void Engine::leave_game(int id) 
{
	if (!verify_game_status(START)) return;
	players_.erase(players_.begin() + id);
}

void Engine::modify_board(int rows, int cols, int total_mines) 
{
	if (!verify_game_status(START)) return;

	try {
		board_->modify_board(rows, cols, total_mines);
	} catch (SweeperException& ex) {
		interaction_->dispatch_error(ex.get_sweeper_error());
	}
}

void Engine::turn_played(int row, int col) 
{
	if (!verify_game_status(RUN)) return;

	list<BoardPosition *> listRevealed;
	try {
		listRevealed = board_->reveal_position(row, col);
	} catch (SweeperException& ex) {
		interaction_->dispatch_error(ex.get_sweeper_error());
		return;
	}

	bool foundMine = false;
	for (BoardPosition * revealed : listRevealed) {
		if (revealed->is_mine()) {
			foundMine = true;
			break;
		}
	}

	if (!foundMine) 
		next_player();
	this->interaction_->board_position_revealed(listRevealed);

	if (is_game_finished()) {
		current_status_ = START;
		current_player_ = nullptr;
		this->interaction_->game_finished();
	}
}

void Engine::surrender(Player player)
{
}

Player* Engine::next_player() 
{
	if (!verify_game_status(RUN)) return nullptr;

	if (current_player_ == nullptr) {
		srand((unsigned int) time(NULL));
		current_player_index_ = rand() % players_.size();
		current_player_ = &players_[current_player_index_];
		return current_player_;
	}
	current_player_ = &players_[++current_player_index_ % players_.size()];
	return current_player_;
}

Player const& Engine::get_current_player() const 
{
	return *current_player_;
}

std::vector<Player> const& Engine::get_players_list() const 
{
	return players_;
}

ostream& operator<<(ostream& os, const Engine& obj) 
{
	os << *(obj.board_);
	return os;
}

bool Engine::is_game_finished() {
	return board_->all_mines_revealed();
}

bool Engine::verify_game_status(Status expected) 
{
	if (expected == current_status_)
		return true;

	if (current_status_ == START)
		interaction_->dispatch_error(SweeperError::GAME_NOT_RUNNING);
	if (current_status_ == RUN)
		interaction_->dispatch_error(SweeperError::GAME_ALREADY_STARTED);
	return false;
}