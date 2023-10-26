#pragma once
#include "../Enums.h"

struct Action {

public:

	unsigned char GetX() const { return m_X; }
	unsigned char GetY() const { return m_Y; }
	int GetUnit1() const { return m_Unit1; }
	int GetUnit2() const { return m_Unit2; }
	ActionType GetActionType() const { return m_ActionType; }
	UnitType GetUnitType() const { return m_UnitType; }

	Action(ActionType actionType, int unit1, int unit2, unsigned char x, unsigned char y, UnitType unitType)
		: m_X(x), m_Y(y), m_Unit1(unit1), m_Unit2(unit2), m_ActionType(actionType), m_UnitType(unitType) {};

	//Move unit to x, y
	Action(int unit, unsigned char x, unsigned char y)
		: m_X(x), m_Y(y), m_Unit1(unit), m_ActionType(Move) {};

	//Create unit_type at x, y
	Action(UnitType unit_type, unsigned char x, unsigned char y)
		: m_X(x), m_Y(y), m_ActionType(Create), m_UnitType(unit_type) {};

	//Attack defender with attacker
	Action(int attacker, int defender)
		: m_Unit1(attacker), m_Unit2(defender), m_ActionType(Attack) {};

	Action() {};

private:
	unsigned char m_X;
	unsigned char m_Y;
	int m_Unit1;
	int m_Unit2;
	ActionType m_ActionType;
	UnitType m_UnitType;
};