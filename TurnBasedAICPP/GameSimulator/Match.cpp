#include "Match.h"
#include "../Serialiser.h"
#include "../GridRenderer.h"

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
	GridRenderer renderer(m_Simulator.GetCurrentState());
	renderer.DrawGrid();

	//m_Simulator.GetCurrentState().DrawGrid();
}

void Match::PrintData() const
{
	int create = 0;
	int move = 0;
	int attack = 0;
	int swap = 0;

	for (const auto& action : m_Actions)
	{
		switch (action.GetActionType())
		{
		case Create:
			create++;
			break;
		case Move:
			move++;
			break;
		case Attack:
			attack++;
			break;
		case Swap:
			swap++;
			break;
		}
	}

	std::cout << "\n\nMatch Data\n__________"
		<< "\nTotal Moves: " << m_Actions.size() << std::endl << std::endl
		<< "\nCreate actions: " << create
		<< "\nMove actions: " << move
		<< "\nAttack actions: " << attack
		<< "\nSwap actions: " << swap << std::endl;
}
