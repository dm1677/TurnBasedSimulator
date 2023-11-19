#pragma once
#include <vector>
#include <string>
#include "GameSimulator/GameState.h"


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
	Puzzle(const GameState& state, const std::vector<Action>& correctActions, const std::string& name = "Puzzle")
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
