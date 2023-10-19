#pragma once
#include "Action.h"
#include "GameState.h"
#include "Unit.h"
#include <vector>

class Simulator
{
public:
	Simulator(GameState& state, Action& action) : m_state(state), m_action(action), m_units(state.GetUnitData()) {};
	GameState GenerateNewState(Action& action);
	GameState GetCurrentState() const { return m_state; }

private:
	GameState& m_state;
	Action& m_action;
	std::vector<Unit> m_units;

	void executeMoveAction();
	void executeAttackAction();
	void executeCreateAction();
};

