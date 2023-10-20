#include "Match.h"

void Match::UpdateState(const Action& action)
{
	m_Simulator.GenerateNewState(action);
	auto state = m_Simulator.GetCurrentState();
	state.DrawGrid();
}