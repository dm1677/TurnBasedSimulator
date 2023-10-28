#pragma once
#include "Simulator.h"
#include <vector>
#include <string>

class Match
{
public:
	void UpdateState(const Action& action);
	void CreateReplayFile(const std::string& filename = "replay.tbr") const;
	void PlayReplayFromFile(const std::string& filename, bool drawState = false);
	const GameState& GetCurrentGameState() const { return m_Simulator.GetCurrentState(); }
	void DrawCurrentState() const;
	void PrintData() const;
private:
	std::vector<Action> m_Actions;
	Simulator m_Simulator;
};