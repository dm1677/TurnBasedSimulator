#include "Simulator.h"

GameState Simulator::GenerateNewState(const Action& action)
{
	if (m_State.IsGameOver()) return m_State;
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
	case Swap:
		success = executeSwapAction();
		break;
	default:
		success = false;
		break;
	}

	if (!success) return m_State;
	m_Units[m_State.GetEnemy()].SetHealth(m_Units[m_State.GetEnemy()].GetHealth() + 1);
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
	for (const auto& unit : m_Units) {
		if (unit.GetX() == destinationX && unit.GetY() == destinationY)
			return false;
	}

	m_Units[m_Action.GetUnit1()].SetX(destinationX);
	m_Units[m_Action.GetUnit1()].SetY(destinationY);
	return true;
}

bool Simulator::executeAttackAction()
{
	auto& attacker = m_Units[m_Action.GetUnit1()];
	auto& defender = m_Units[m_Action.GetUnit2()];
	if (!defender.TakeDamage(attacker.GetDamage())) return true;

	if (attacker.IsMoveAttacker()) {
		attacker.SetX(defender.GetX());
		attacker.SetY(defender.GetY());
	}

	m_Units.erase(m_Units.begin() + m_Action.GetUnit2());
	//defender.SetX(15);
	//defender.SetY(15);
	//defender.SetHealth(0);
	return true;
}

bool Simulator::executeCreateAction()
{
	auto destinationX = m_Action.GetX();
	auto destinationY = m_Action.GetY();

	//Error code here
	for (const auto& unit : m_Units) {
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

bool Simulator::executeSwapAction()
{
	auto& prawn = m_Units[m_Action.GetUnit1()];
	auto& swappedUnit = m_Units[m_Action.GetUnit2()];

	if (prawn.GetUnitType() != Prawn) return false;
	if (swappedUnit.GetUnitType() == Resource || swappedUnit.GetUnitType() == Tree) return false;
	if (prawn.GetOwner() != m_State.GetPlayer()) return false;
	if (swappedUnit.GetOwner() != m_State.GetPlayer()) return false;
	if (m_Action.GetUnit1() == m_Action.GetUnit2()) return false;

	Pos prawnPos(prawn.GetX(), prawn.GetY());

	prawn.SetX(swappedUnit.GetX());
	prawn.SetY(swappedUnit.GetY());

	swappedUnit.SetX(prawnPos.X);
	swappedUnit.SetY(prawnPos.Y);

	return true;
}