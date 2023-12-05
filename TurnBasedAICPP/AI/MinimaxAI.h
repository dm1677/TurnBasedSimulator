#pragma once
#include "AI.h"
#include "../GameSimulator/GameState.h"
#include "../GameSimulator/Action.h"
#include <iostream>
#include <vector>

namespace TurnBasedSimulator
{
	class MinimaxAI
		: public AI {
	public:
		explicit MinimaxAI(const GameState& state)
			:m_State(state) {}
		Action GetAction() const override;

	private:
		const GameState& m_State;



		class Node {
		public:
			explicit Node(const GameState& state)
				: m_State(std::make_unique<GameState>(state)) {}
			int Minimax(int depth, bool maximiser, int alpha = INT32_MIN, int beta = INT32_MAX);
			int GetEvaluation() const { return m_Evaluation; }
			Action GetBestAction() const { return m_BestAction; }
		private:
			const std::unique_ptr<GameState> m_State;
			std::vector<std::unique_ptr<Action>> m_Actions;
			std::vector<std::unique_ptr<Node>> m_Children;
			int m_Evaluation = 0;
			Action m_BestAction;

			int getEvaluation() const;
			void populateActions();
			void populateChildren();
		};
	};
}