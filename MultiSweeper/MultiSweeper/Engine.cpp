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
	kMaxPlayers(4), current_status_(START), current_player_index_(0),
	board_(make_unique<Board>(rows, cols, mines)), interaction_(interaction)
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

	this->interaction_->game_started();
}

void Engine::join_game(string username)
{
	if (!verify_game_status(START)) return;
	if (players_.size() >= kMaxPlayers) {
		interaction_->dispatch_error(SweeperError::MAX_PLAYERS_REACHED);
		return;
	}
	if (username.length() < 3 || username.length() > 20) {
		interaction_->dispatch_error(SweeperError::PLAYER_USERNAME_INVALID);
		return;
	}

	Player player(username);
	if (std::find(players_.begin(), players_.end(), player) != players_.end()) {
		interaction_->dispatch_error(SweeperError::PLAYER_ALREADY_EXISTS);
		return;
	}
	players_.push_back(player);
}

void Engine::leave_game(string username)
{
	if (!verify_game_status(START)) return;
	auto it = std::find(players_.begin(), players_.end(), username);
	if (it != players_.end()) {
		players_.erase(it);
	}
	else {
		interaction_->dispatch_error(SweeperError::PLAYER_NOT_EXISTS);
	}
}

void Engine::leave_game(int id)
{
	if (!verify_game_status(START)) return;
	if (id >= players_.size()) {
		interaction_->dispatch_error(SweeperError::PLAYER_NOT_EXISTS);
	}
	else {
		players_.erase(players_.begin() + id);
	}
}

void Engine::modify_board(int rows, int cols, int total_mines)
{
	if (!verify_game_status(START)) return;

	try {
		board_->modify_board(rows, cols, total_mines);
	}
	catch (SweeperException& ex) {
		interaction_->dispatch_error(ex.get_sweeper_error());
	}
}

void Engine::turn_played(int row, int col)
{
	if (!verify_game_status(RUN)) return;

	list<BoardPosition *> listRevealed;
	try {
		listRevealed = board_->reveal_position(row, col);
	}
	catch (SweeperException& ex) {
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

	if (!foundMine) {
		players_[current_player_index_].increase_mines_missed();
		next_player();
	} else {
		players_[current_player_index_].increase_mines_revealed();
	}
	this->interaction_->board_position_revealed(listRevealed);

	if (is_game_finished()) {
		vector<Player *> winners;
		winners.push_back(&players_[0]);
		for (int i = 1; i < players_.size(); i++) {
			if (players_[i].get_mines_revealed() > winners.back()->get_mines_revealed()) {
				for (auto winner : winners)
					winner->update_game_stats(false);
				winners.clear();
				winners.push_back(&players_[i]);
			} 
			else if (players_[i].get_mines_revealed() < winners.back()->get_mines_revealed()) {
				players_[i].update_game_stats(false);
			}
			else {
				winners.push_back(&players_[i]);
			}
		}
		// If vector only has 1 element, that player has won
		// Otherwise, they tied.
		vector<const Player*> winners_const;
		for (auto winner : winners) {
			winner->update_game_stats(winners.size() == 1);
			winners_const.push_back(winner);
		}
		
		this->interaction_->player_won(winners_const);
		this->current_status_ = START;
		this->current_player_index_ = 0;
		this->interaction_->game_finished();
		this->board_->generate_mines();
	}
}

void Engine::surrender(Player player)
{
}

Player & Engine::next_player()
{
	if (!verify_game_status(RUN)) return players_[current_player_index_];

	current_player_index_ = ++current_player_index_ % players_.size();
	return players_[current_player_index_];
}

Board const& Engine::get_board() const
{
	return *board_;
}

int const& Engine::get_current_player_index() {
	return current_player_index_;
}

std::vector<Player> const& Engine::get_players_list() const
{
	return players_;
}

int const& Engine::get_max_players() const {
	return kMaxPlayers;
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