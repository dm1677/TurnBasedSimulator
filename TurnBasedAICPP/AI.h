#pragma once
#include "GameState.h"
#include <iostream>
#include <vector>

class AI {
public:
	//AI(GameState board, Action action) :current_state(board), parent_move(action) {};
	AI(GameState* board, Action& action);
	~AI();
	Action get_move();

private:
	int get_evaluation();
	void populate_actions();
	void populate_child_nodes();
	void flush();

	std::vector<AI*> children;
	std::vector<Action> moves;

	Action parent_move;

	int evaluation;

	GameState* current_state;

	int minimax(int, int, int, bool);
};