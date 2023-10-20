#include "Simulator.h"

GameState Simulator::GenerateNewState(const Action& action)
{
	m_Action = action;
	switch (m_Action.GetActionType())
	{
	case Move:
		executeMoveAction();
		break;
	case Attack:
		executeAttackAction();
		break;
	case Create:
		executeCreateAction();
		break;
	}

	m_State = GameState(m_Units, m_State.GetEnemy());
	return m_State;
}

void Simulator::executeMoveAction()
{
	auto destinationX = m_Action.GetX();
	auto destinationY = m_Action.GetY();

	//Add some error handling
	if(!GameState::IsInBounds(destinationX, destinationY)) return;
	for (auto unit : m_Units) {
		if (unit.GetX() == destinationX && unit.GetY() == destinationY)
			return;
	}

	m_Units[m_Action.GetUnit1()].SetX(destinationX);
	m_Units[m_Action.GetUnit1()].SetY(destinationY);
}

void Simulator::executeAttackAction()
{
	auto attacker = m_Units[m_Action.GetUnit1()];
	auto defender = m_Units[m_Action.GetUnit2()];
	if (defender.TakeDamage(attacker.GetDamage())) return;

	if (attacker.IsMoveAttacker()) {
		attacker.SetX(defender.GetX());
		attacker.SetY(defender.GetY());
	}

	m_Units.erase(m_Units.begin() + m_Action.GetUnit2());
}

void Simulator::executeCreateAction()
{
	auto destinationX = m_Action.GetX();
	auto destinationY = m_Action.GetY();

	//Error code here
	for (auto unit : m_Units) {
		if (unit.GetX() == destinationX && unit.GetY() == destinationY)
			return;
	}

	auto money = m_State.GetMoney(m_State.GetPlayer());
	auto cost = Unit::GetCost(m_Action.GetUnitType());
	if (money < cost)
		return;
	//

	m_Units[m_State.GetPlayer()].SetHealth(money - cost);

	Unit unit(destinationX,
	          destinationY,
		      Unit::GetMaxHealth(m_Action.GetUnitType()),
		      (int)m_Action.GetUnitType(),
		      (int)m_State.GetPlayer());
	m_Units.push_back(unit);
}