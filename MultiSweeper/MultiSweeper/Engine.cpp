#include "Engine.h"

#include <string>
#include <random>
#include <time.h>
#include <list>
#include <algorithm>
#include "SweeperException.h"
#include "SweeperError.h"

using namespace std;

Engine::Engine(InterfaceVisual* interaction) : Engine(interaction, 10, 5, 10)
{
}

Engine::Engine(InterfaceVisual* interaction, int rows, int cols, int mines) :
	kMaxPlayers(4), current_status_(START), current_player_index_(0),
	board_(make_unique<Board>(rows, cols, mines)), interaction_(interaction)
{
}

Engine::~Engine() 
{
}

Engine::Engine(const Engine & other) : kMaxPlayers(other.kMaxPlayers)
{
	*this = other;
}

Engine::Engine(Engine&& other) : kMaxPlayers(other.kMaxPlayers),
	current_status_(other.current_status_), current_player_index_(other.current_player_index_),
	players_(other.players_), interaction_(other.interaction_)
{
	this->board_ = std::move(other.board_);
}

Engine& Engine::operator=(const Engine& other)
{
	if (this != &other) {
		this->current_status_ = other.current_status_;
		this->current_player_index_ = other.current_player_index_;
		this->players_ = other.players_;
		this->interaction_ = other.interaction_;
		this->board_ = make_unique<Board>(*(other.board_));
	}
	return *this;
}

Engine& Engine::operator=(Engine&& other)
{
	if (this != &other) {
		this->current_status_ = other.current_status_;
		this->current_player_index_ = other.current_player_index_;
		this->players_ = other.players_;
		this->interaction_ = other.interaction_;
		this->board_ = std::move(other.board_);
	}
	return *this;
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
	players_.push_back(std::move(player));
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

	bool found_mine = false;
	for (BoardPosition * revealed : listRevealed) {
		if (revealed->is_mine()) {
			found_mine = true;
			break;
		}
	}

	if (!found_mine) {
		players_[current_player_index_].increase_mines_missed();
		next_player();
	} else {
		players_[current_player_index_].increase_mines_revealed();
	}
	this->interaction_->board_position_revealed(listRevealed);

	if (is_game_finished()) {
		game_finished();
	}
}

void Engine::game_finished() 
{
	vector<Player *> winners;
	for (Player& p : players_)
	{
		if (p.has_surrendered()) 
		{
			p.update_game_stats(false);
			continue;
		}

		if (winners.empty()) {
			winners.push_back(&p);
			continue;
		}

		if (p.get_mines_revealed() > winners.back()->get_mines_revealed()) {
			for (auto winner : winners)
				winner->update_game_stats(false);
			winners.clear();
			winners.push_back(&p);
		}
		else if (p.get_mines_revealed() < winners.back()->get_mines_revealed()) {
			p.update_game_stats(false);
		}
		else {
			winners.push_back(&p);
		}
	}
	// If vector only has 1 element, that player has won
	// Otherwise, they tied.
	vector<const Player*> winners_const;
	for (const auto &winner : winners) {
		winner->update_game_stats(winners.size() == 1);
		winners_const.push_back(winner);
	}

	this->interaction_->player_won(winners_const);
	this->current_status_ = START;
	this->current_player_index_ = 0;
	this->interaction_->game_finished();
	this->board_->generate_mines();
}

void Engine::surrender()
{
	if (!verify_game_status(RUN)) return;

	players_[current_player_index_].surrender();
	if (is_game_finished()) {
		game_finished();
	}
	else {
		next_player();
	}
}

Player & Engine::next_player()
{
	if (!verify_game_status(RUN)) return players_[current_player_index_];

	do {
		current_player_index_ = ++current_player_index_ % players_.size();
	} while (players_[current_player_index_].has_surrendered());

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
	bool all_players_surrendered = true;
	for (Player p : players_)
	{
		if (!p.has_surrendered())
			all_players_surrendered = false;
	}
	return board_->all_mines_revealed() | all_players_surrendered;
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