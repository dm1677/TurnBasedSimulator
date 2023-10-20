#pragma once
#include "Simulator.h"
#include <vector>

class Match
{
public:
	void UpdateState(const Action& action);
	void CreateReplayFile() const;
private:
	std::vector<Action> m_Actions;
	Simulator m_Simulator;
};