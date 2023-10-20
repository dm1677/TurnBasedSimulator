#include "Match.h"
#include "Serialiser.h"

void Match::UpdateState(const Action& action)
{
	m_Simulator.GenerateNewState(action);
	m_Actions.push_back(action);
	auto state = m_Simulator.GetCurrentState();
	state.DrawGrid();
}

void Match::CreateReplayFile() const
{
	//if (m_Simulator.GetCurrentState().IsGameOver())
	{
		Serialiser serialiser;
		serialiser.WriteReplayToFile(m_Actions, "replay.tbr");
	}
}

void Match::PlayReplayFromFile(const std::string& filename)
{
	if (m_Actions.empty())
	{
		Serialiser serialiser;
		auto actions = serialiser.ReadReplayFile(filename);
		if (actions == nullptr) return;
		for (const Action& action : *actions)
		{
			UpdateState(action);
		}
		delete actions;
	}
}