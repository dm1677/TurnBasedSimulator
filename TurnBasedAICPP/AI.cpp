/*#include "ai.h"
#include <algorithm>
#include <assert.h>
using std::max;
using std::min;

Action AI::get_move() {
	minimax(2, -2, 2, true);
	int best = -2;
	Action best_move(0,0);

	for (uint32_t i = 0; i < children.size(); i++)
	{
		if(children[i]->evaluation > best)
		{
			best = children[i]->evaluation;
			best_move = children[i]->parent_move;
		}
	}

	flush();

	assert(best_move.GetUnit1() != 0 && best_move.GetUnit2() != 0);

	return best_move;
}

void AI::flush() {
	children.clear();
	moves.clear();
}

int AI::minimax(int depth, int alpha, int beta, bool maximiser)
{
	if (depth == 0) { return get_evaluation(); }
	populate_actions();
	if (moves.size() == 0) { return get_evaluation(); }
	if (current_state->game_over()) { return get_evaluation(); }
	populate_child_nodes();
	if (maximiser)
	{
		int best = -2;

		for (unsigned int i = 0; i < children.size(); i++) {
			current_state->make_move(children[i]->parent_move);

			best = max(best, children[i]->minimax(depth - 1, alpha, beta, false));
			
			current_state->undo_move();

			alpha = max(alpha, best);
			if (alpha >= beta) break;
		}
		
		evaluation = best;
		return best;
	}
	else
	{
		int best = 2;
		for (unsigned int i = 0; i < children.size(); i++) {
			current_state->make_move(children[i]->parent_move);

			best = min(best, children[i]->minimax(depth - 1, alpha, beta, true));			

			current_state->undo_move();

			beta = min(beta, best);
			if (beta <= alpha) break;
		}
		
		evaluation = best;
		return best;
	}
}

int AI::get_evaluation() {	
	User result = (User)current_state->result();

	switch (result)
	{
	case Neutral:
		return 0;
	case Enemy:
		return 1;
	case Player:
		return -1;
	default:
		return 0;
	}
}

void AI::populate_actions() {
	moves = current_state->get_legal_moves();
}

void AI::populate_child_nodes() {//fix
	for (unsigned i = 0; i < moves.size(); i++)
	{
		//GameState new_board(current_state, moves[i]);

		AI* new_ai = new AI(current_state, moves[i]);
		children.push_back(new_ai);
	}
}

AI::AI(GameState *board, Action &action) {
	current_state = board;
	parent_move = action;	
}

AI::~AI() {
	for (auto child : children)
	{
		delete child;
	}
}*/