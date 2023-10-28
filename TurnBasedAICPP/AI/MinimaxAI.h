#pragma once
#include "AI.h"
#include "../GameSimulator/GameState.h"
#include "../GameSimulator/Action.h"
#include <iostream>
#include <vector>

class MinimaxAI
: public AI {
public:
	explicit MinimaxAI(const GameState& state)
		:m_State(state) {}
	Action GetMove();

private:
	const GameState& m_State;



	class Node {
	public: 
		explicit Node(const GameState& state)
			: m_State(state), m_Evaluation(getEvaluation()) {}
		int Minimax(int depth, int alpha, int beta, bool maximiser);
		int GetEvaluation() const { return m_Evaluation; }
		Action GetBestAction() const { return m_BestAction; }
	private:
		const GameState& m_State;
		std::vector<Action> m_Actions;
		std::vector<Node> m_Children;
		int m_Evaluation;
		Action m_BestAction;

		int getEvaluation() const;
		void populateActions();
		void populateChildren();
	};
};