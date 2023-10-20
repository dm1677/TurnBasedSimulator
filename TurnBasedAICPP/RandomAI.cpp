#include "RandomAI.h"
#include <random>

Action RandomAI::GetAction() const
{
	auto actions = m_State.GetLegalMoves();
	
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, actions.size()-1);
	
	int randomNumber = dist(rng);

	if (actions.empty()) return Action();
	return actions[randomNumber];
}