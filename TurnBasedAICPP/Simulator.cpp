#include "Simulator.h"

GameState Simulator::GenerateNewState(const Action& action)
{
	m_Action = action;
	bool success;
	switch (m_Action.GetActionType())
	{
	case Move:
		success = executeMoveAction();
		break;
	case Attack:
		success = executeAttackAction();
		break;
	case Create:
		success = executeCreateAction();
		break;
	default:
		success = false;
	}

	if (!success) return m_State;
	m_State = GameState(m_Units, m_State.GetEnemy());
	return m_State;
}

bool Simulator::executeMoveAction()
{
	auto destinationX = m_Action.GetX();
	auto destinationY = m_Action.GetY();

	//Add some error handling
	if(!GameState::IsInBounds(destinationX, destinationY)) return false;
	if (m_Units[m_Action.GetUnit1()].GetOwner() != m_State.GetPlayer()) return false;
	for (auto unit : m_Units) {
		if (unit.GetX() == destinationX && unit.GetY() == destinationY)
			return false;
	}

	m_Units[m_Action.GetUnit1()].SetX(destinationX);
	m_Units[m_Action.GetUnit1()].SetY(destinationY);
	return true;
}

bool Simulator::executeAttackAction()
{
	auto attacker = m_Units[m_Action.GetUnit1()];
	auto defender = m_Units[m_Action.GetUnit2()];
	if (!defender.TakeDamage(attacker.GetDamage())) return false;

	if (attacker.IsMoveAttacker()) {
		attacker.SetX(defender.GetX());
		attacker.SetY(defender.GetY());
	}

	m_Units.erase(m_Units.begin() + m_Action.GetUnit2());
	return true;
}

bool Simulator::executeCreateAction()
{
	auto destinationX = m_Action.GetX();
	auto destinationY = m_Action.GetY();

	//Error code here
	for (auto unit : m_Units) {
		if (unit.GetX() == destinationX && unit.GetY() == destinationY)
			return false;
	}

	auto money = m_State.GetMoney(m_State.GetPlayer());
	auto cost = Unit::GetCost(m_Action.GetUnitType());
	if (money < cost)
		return false;
	//

	m_Units[m_State.GetPlayer()].SetHealth(money - cost);

	Unit unit(destinationX,
	          destinationY,
		      Unit::GetMaxHealth(m_Action.GetUnitType()),
		      m_Action.GetUnitType(),
		      m_State.GetPlayer());
	m_Units.push_back(unit);
	return true;
}