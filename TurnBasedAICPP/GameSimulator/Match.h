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
	void LoadReplayFromFile(const std::string& filename);
	void AdvanceReplay();
	const GameState& GetCurrentGameState() const { return m_Simulator.GetCurrentState(); }
	void DrawCurrentState() const;
	void PrintData() const;
	bool IsGameOver() const { return m_Simulator.GetCurrentState().IsGameOver(); }
	User GetWinner() const { return (User)m_Simulator.GetCurrentState().GetResult(); }
private:
	std::vector<Action> m_Actions;
	Simulator m_Simulator;
	std::vector<Action>::iterator m_ReplayAction;
};