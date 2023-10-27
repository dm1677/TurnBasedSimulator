#include "Match.h"
#include "../Serialiser.h"

void Match::UpdateState(const Action& action)
{
	m_Simulator.GenerateNewState(action);
	m_Actions.push_back(action);
}

void Match::CreateReplayFile(const std::string& filename) const
{
	//if (m_Simulator.GetCurrentState().IsGameOver())
	{
		Serialiser serialiser;
		serialiser.WriteReplayToFile(m_Actions, filename);
	}
}

void Match::PlayReplayFromFile(const std::string& filename, bool drawState)
{
	if (m_Actions.empty())
	{
		Serialiser serialiser;
		auto actions = serialiser.ReadReplayFile(filename);
		
		int i = 0;
		for (const Action& action : actions)
		{
			UpdateState(action);
			if (drawState) { std::cout << "Move " << i++ << std::endl; DrawCurrentState(); }
		}
	}
}

void Match::DrawCurrentState() const
{
	m_Simulator.GetCurrentState().DrawGrid();
}