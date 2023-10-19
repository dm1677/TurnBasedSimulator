#pragma once
#include "Enums.h"
#include "Unit.h"
#include "Action.h"
#include <vector>
#include <iostream>
#include <stack>

struct GameState {
public:
	GameState() {
		create_units();
		player_to_move = Player;
	};
	explicit GameState(const std::vector<Unit>& units, User player) : units(units), player_to_move(player) {};

	User GetPlayer() const { return player_to_move; }
	User GetEnemy() const;

	int GetMoney(User user) const;

	static bool IsInBounds(int x, int y);
	bool passable(int x, int y) const;
	bool game_over();
	int result();

	std::vector<Action> GetLegalMoves();
	std::vector<std::pair<int, int>> get_movement(const Unit &unit);
	std::vector<int> get_attacks(const Unit &unit);

	void print_units();
	void DrawGrid();

	std::vector<Unit> GetUnitData() const { return units; }
private:
	std::vector<Unit> units;
	std::stack<Action> actions;
	User player_to_move;

	void create_units();
};

