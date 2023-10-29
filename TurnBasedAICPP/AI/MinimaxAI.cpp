#include "MinimaxAI.h"
#include "../GameSimulator/Simulator.h"

using std::max;
using std::min;

Action MinimaxAI::GetAction() const
{
	Node node(m_State);
	node.Minimax(3, true);
	return node.GetBestAction();
}

int MinimaxAI::Node::Minimax(int depth, bool maximiser, int alpha, int beta)
{
	if (depth == 0 || m_State->IsGameOver()) return getEvaluation();
	populateActions();
	if (m_Actions.empty()) return getEvaluation();

	populateChildren();

	if (maximiser)
	{
		int best = INT32_MIN;
		for (int i = 0; i < m_Children.size(); i++) {
			const auto& childNode = m_Children[i];
			int childScore = childNode->Minimax(depth - 1, false, alpha, beta);
			if (childScore > best) {
				best = childScore;
				m_BestAction = *m_Actions[i];
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
			const auto& childNode = m_Children[i];
			int childScore = childNode->Minimax(depth - 1, true, alpha, beta);
			if (childScore < best) {
				best = childScore;
				m_BestAction = *m_Actions[i];
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
	if ((User)m_State->GetResult() == m_State->GetPlayer()) return INT32_MAX;
	if ((User)m_State->GetResult() == m_State->GetEnemy())  return INT32_MIN;

	int playerKingHealthPool = 0;
	int enemyKingHealthPool = 0;
	const auto& kings = m_State->GetKings();

	/*const std::array<Vec2, 8> directionVectors = { {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}} };

	int playerFreeAdjacentSquares = 0;
	int enemyFreeAdjacentSquares = 0;*/

	for (const auto& king : kings)
	{
		if (king.GetOwner() == m_State->GetPlayer())
		{
			playerKingHealthPool += king.GetHealth();
			//for (const auto& vector : directionVectors)
			//{
			//	const int x = king.GetX() + vector.X;
			//	const int y = king.GetY() + vector.Y;
			//	if (!GameState::IsInBounds(x, y) || !m_State->IsPassable(x, y)) continue;
			//	playerFreeAdjacentSquares++;
			//}
		}
		if (king.GetOwner() == m_State->GetEnemy())
		{
			enemyKingHealthPool += king.GetHealth();
			//for (const auto& vector : directionVectors)
			//{
			//	const int x = king.GetX() + vector.X;
			//	const int y = king.GetY() + vector.Y;
			//	if (!GameState::IsInBounds(x, y) || !m_State->IsPassable(x, y)) continue;
			//	enemyFreeAdjacentSquares++;
			//}
		}
	}

	return (enemyKingHealthPool - playerKingHealthPool);// + (enemyFreeAdjacentSquares - playerFreeAdjacentSquares);
}

void MinimaxAI::Node::populateActions()
{
	std::vector<Action> actions = m_State->GetLegalMoves();
	for (const auto& action : actions)
		m_Actions.emplace_back(std::make_unique<Action>(action));
}

void MinimaxAI::Node::populateChildren()
{
	for (const auto& action : m_Actions)
	{
		Simulator simulator(*m_State, *action);
		m_Children.emplace_back(std::make_unique<Node>(simulator.GenerateNewState(*action)));
	}
}
