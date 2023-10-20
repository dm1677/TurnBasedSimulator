#pragma once
#include "Simulator.h"
#include <vector>
#include <string>

class Match
{
public:
	void UpdateState(const Action& action);
	void CreateReplayFile() const;
	void PlayReplayFromFile(const std::string& filename);
	GameState GetCurrentGameState() const { return m_Simulator.GetCurrentState(); }
private:
	std::vector<Action> m_Actions;
	Simulator m_Simulator;
};