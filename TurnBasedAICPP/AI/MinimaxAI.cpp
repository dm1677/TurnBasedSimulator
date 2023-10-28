#include "MinimaxAI.h"
#include "../GameSimulator/Simulator.h"

using std::max;
using std::min;

Action MinimaxAI::GetMove()
{
	Node node = Node(m_State);
	return node.GetBestAction();
}
int MinimaxAI::Node::Minimax(int depth, int alpha, int beta, bool maximiser)
{
	if (depth == 0 || m_State.IsGameOver()) return getEvaluation();
	populateActions();
	if (m_Actions.empty()) return getEvaluation();

	populateChildren();

	if (maximiser)
	{
		int best = INT32_MIN;
		for (int i = 0; i < m_Children.size(); i++) {
			auto& childNode = m_Children[i];
			int childScore = childNode.Minimax(depth - 1, alpha, beta, false);
			if (childScore > best) {
				best = childScore;
				m_BestAction = m_Actions[i];
			}
			alpha = max(alpha, best);
			if (alpha >= beta) break;
		}
		m_Evaluation = best;
		return best;
	}
	else
	{
		int best = INT32_MAX;
		for (int i = 0; i < m_Children.size(); i++) {
			auto& childNode = m_Children[i];
			int childScore = childNode.Minimax(depth - 1, alpha, beta, true);
			if (childScore < best) {
				best = childScore;
				m_BestAction = m_Actions[i];
			}
			beta = min(beta, best);
			if (beta <= alpha) break;
		}
		m_Evaluation = best;
		return best;
	}
}

int MinimaxAI::Node::getEvaluation() const
{
	if ((User)m_State.GetResult() == m_State.GetPlayer()) return INT32_MAX;
	if ((User)m_State.GetResult() == m_State.GetEnemy())  return INT32_MIN;

	int playerKingHealthPool = 0;
	int enemyKingHealthPool = 0;
	const auto& kings = m_State.GetKings();
	for (const auto& king : kings)
	{
		if (king.GetOwner() == m_State.GetPlayer()) playerKingHealthPool += king.GetHealth();
		if (king.GetOwner() == m_State.GetEnemy())  enemyKingHealthPool  += king.GetHealth();
	}

	return playerKingHealthPool - enemyKingHealthPool;
}

void MinimaxAI::Node::populateActions()
{
	m_Actions = m_State.GetLegalMoves();
}

void MinimaxAI::Node::populateChildren()
{
	for (const auto& action : m_Actions)
	{
		Simulator simulator(m_State, action);
		m_Children.emplace_back(Node(simulator.GenerateNewState(action)));
	}
}
