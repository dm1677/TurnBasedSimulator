#include "Simulator.h"

GameState Simulator::GenerateNewState(Action& action)
{
	m_action = action;
	switch (m_action.GetActionType())
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

	m_state = GameState(m_units, m_state.GetEnemy());
	return m_state;
}

void Simulator::executeMoveAction()
{
	auto destinationX = m_action.GetX();
	auto destinationY = m_action.GetY();

	//Add some error handling
	if(!m_state.IsInBounds(destinationX, destinationY)) return;
	for (auto unit : m_units) {
		if (unit.GetX() == destinationX && unit.GetY() == destinationY)
			return;
	}

	m_units[m_action.GetUnit1()].setX(destinationX);
	m_units[m_action.GetUnit1()].setY(destinationY);
}

void Simulator::executeAttackAction()
{
	auto attacker = m_units[m_action.GetUnit1()];
	auto defender = m_units[m_action.GetUnit2()];
	if (defender.TakeDamage(attacker.GetDamage())) return;

	if (attacker.IsMoveAttacker()) {
		attacker.setX(defender.GetX());
		attacker.setY(defender.GetY());
	}

	m_units.erase(m_units.begin() + m_action.GetUnit2());
}

void Simulator::executeCreateAction()
{
	auto destinationX = m_action.GetX();
	auto destinationY = m_action.GetY();

	//Error code here
	for (auto unit : m_units) {
		if (unit.GetX() == destinationX && unit.GetY() == destinationY)
			return;
	}

	int money = m_state.GetMoney(m_state.GetPlayer());
	int cost = Unit::get_cost(m_action.GetUnitType());
	if (money < cost)
		return;
	//

	m_units[m_state.GetPlayer()].setHealth(money - cost);

	Unit unit(destinationX,
	          destinationY,
		      Unit::GetMaxHealth(m_action.GetUnitType()),
		      (int)m_action.GetUnitType(),
		      (int)m_state.GetPlayer());
	m_units.push_back(unit);
}