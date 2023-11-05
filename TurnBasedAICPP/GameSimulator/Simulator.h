#pragma once
#include "Action.h"
#include "GameState.h"
#include "Unit.h"
#include <vector>

class Simulator
{
public:
	Simulator() : m_State(GameState()), m_Action(Action()), m_Units(m_State.CopyUnitData()) {};
	//Simulator(GameState& state, Action& action) : m_State(state), m_Action(action), m_Units(state.CopyUnitData()) {};
	Simulator(GameState state, Action action) : m_State(state), m_Action(action), m_Units(state.CopyUnitData()) {};
	GameState GenerateNewState(const Action& action);
	const GameState& GetCurrentState() const { return m_State; }

private:
	GameState m_State;
	Action m_Action;
	std::vector<Unit> m_Units;

	bool executeMoveAction();
	bool executeAttackAction();
	bool executeCreateAction();
	bool executeSwapAction();
};

