#pragma once
#include "../GameSimulator/GameState.h"
#include "../AI/AI.h"

struct Puzzle
{
public:
	Puzzle(int player1Money, int player2Money, const std::vector<Unit>& units, const std::vector<Action>& correctActions, const std::string& name = "Puzzle")
		: m_CorrectActions(correctActions), m_Name(name)
	{
		auto initialUnits = std::vector<Unit>();
		initialUnits.reserve(2 + units.size());
		initialUnits.emplace_back(15, 15, player1Money, Resource, Player);
		initialUnits.emplace_back(15, 15, player2Money, Resource, Enemy);

		for (Unit unit : units)
			initialUnits.push_back(unit);

		m_State = GameState(initialUnits, Player);
	}
	Puzzle (const GameState& state, const std::vector<Action>& correctActions, const std::string& name = "Puzzle")
		: m_State(state), m_CorrectActions(correctActions), m_Name(name) {}

	bool IsCorrectAction(const Action& action) const
	{ 
		auto it = std::find_if(m_CorrectActions.begin(), m_CorrectActions.end(), 
			[&action](const Action& correctAction) { return (action == correctAction); });
		if (it != m_CorrectActions.end()) return true;
		return false;
	}

	const GameState& GetState() const { return m_State; }
	const std::vector<Action>& GetCorrectActions() const { return m_CorrectActions; }
	const std::string& GetName() const { return m_Name; }
private:
	GameState m_State;
	std::vector<Action> m_CorrectActions;
	std::string m_Name;
};

class Test
{
public:
	explicit Test(const Puzzle& puzzle, const std::string& name = "Test")
		: m_Puzzle(puzzle), m_Name(name) {}

	const GameState& GetState() const { return m_Puzzle.GetState(); }
	std::vector<Action> Run(const AI* ai, int attempts = 1) const
	{
		int successes = 0;
		auto failedActions = std::vector<Action>();
		for (int i = 0; i < attempts; i++)
		{
			auto action = ai->GetAction();
			if (m_Puzzle.IsCorrectAction(action))
				successes++;
			else
				failedActions.push_back(action);
		}

		std::cout << successes << "/" << attempts << " tests succeeded." << std::endl 
			<< 100 * (double)successes / (double)attempts << "% of tests passed." << std::endl;

		return failedActions;
	}

private:
	Puzzle m_Puzzle;
	std::string m_Name;
};