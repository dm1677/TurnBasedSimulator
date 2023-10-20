#pragma once
#include "../GameSimulator/GameState.h"
#include "../GameSimulator/Action.h"
#include <iostream>
#include <vector>

class MinimaxAI {
public:
	//AI(GameState board, Action action) :current_state(board), parent_move(action) {};
	MinimaxAI(GameState* board, Action& action);
	~MinimaxAI();
	Action get_move();

private:
	int get_evaluation();
	void populate_actions();
	void populate_child_nodes();
	void flush();

	std::vector<MinimaxAI*> children;
	std::vector<Action> moves;

	Action parent_move;

	int evaluation;

	GameState* current_state;

	int minimax(int, int, int, bool);
};