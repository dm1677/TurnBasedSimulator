#include "RandomAI.h"
#include <random>
#include <map>

Action RandomAI::GetAction() const
{
	if (m_UseHeuristic) return getSemirandomAction();
	auto actions = m_State.GetLegalMoves();
	
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, actions.size()-1);
	
	int randomNumber = dist(rng);

	if (actions.empty()) return Action();
	return actions[randomNumber];
}

Action RandomAI::getSemirandomAction() const
{
	std::map<ActionType, double> c_Weights =
	{
		{Create, 1},
		{Move, 10},
		{Attack, 1000},
		{Swap, 10}
	};


	auto actions = m_State.GetLegalMoves();

	std::vector<double> c_DistributionWeights;
	for (const auto& action : actions)
		c_DistributionWeights.emplace_back(c_Weights[action.GetActionType()]);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> distribution(c_DistributionWeights.begin(), c_DistributionWeights.end());

	if (actions.empty()) return Action();
	return actions[distribution(gen)];
}
