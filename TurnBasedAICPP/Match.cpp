#include "Match.h"

void Match::UpdateState(const Action& action)
{
	m_Simulator.GenerateNewState(action);
	m_Actions.push_back(action);
	auto state = m_Simulator.GetCurrentState();
	state.DrawGrid();
}