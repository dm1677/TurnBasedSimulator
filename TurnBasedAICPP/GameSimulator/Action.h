#pragma once
#include "../Enums.h"
#include <cstdint>

struct Action {

public:

	unsigned char GetX() const { return m_X; }
	unsigned char GetY() const { return m_Y; }
	int GetUnit1() const { return m_Unit1; }
	int GetUnit2() const { return m_Unit2; }
	ActionType GetActionType() const { return m_ActionType; }
	UnitType GetUnitType() const { return m_UnitType; }

	Action(ActionType actionType, int unit1, int unit2, unsigned char x=15, unsigned char y=15, UnitType unitType=Prawn)
		: m_X(x), m_Y(y), m_Unit1(unit1), m_Unit2(unit2), m_ActionType(actionType), m_UnitType(unitType) {};

	//Move unit to x, y
	Action(int unit, unsigned char x, unsigned char y)
		: m_X(x), m_Y(y), m_Unit1(unit), m_Unit2(-1), m_ActionType(Move), m_UnitType(Prawn) {};

	//Create unit_type at x, y
	Action(UnitType unit_type, unsigned char x, unsigned char y)
		: m_X(x), m_Y(y), m_Unit1(-1), m_Unit2(-1), m_ActionType(Create), m_UnitType(unit_type) {};

	//Attack defender with attacker
	Action(int attacker, int defender)
		: m_X(15), m_Y(15), m_Unit1(attacker), m_Unit2(defender), m_ActionType(Attack), m_UnitType(Prawn) {};

	Action(uint32_t packedData)
		: m_X(packedData & 0x0F), m_Y((packedData >> 4) & 0x0F), m_Unit1((packedData >> 8) & 0xFF), m_Unit2((packedData >> 16) & 0xFF),
		  m_ActionType((ActionType)((packedData >> 24) & 0x0F)), m_UnitType((UnitType)((packedData >> 28) & 0x0F)) {};

	Action() = default;

	bool operator==(const Action& other) const
	{
		return m_X == other.m_X
			&& m_Y == other.m_Y
			&& m_Unit1 == other.m_Unit1
			&& m_Unit2 == other.m_Unit2
			&& m_ActionType == other.m_ActionType
			&& m_UnitType == other.m_UnitType;
	}

	uint32_t ToBinary() const
	{
		uint32_t packedData = 0;

		packedData |= (GetX() & 0x0F);
		packedData |= ((GetY() & 0x0F) << 4);
		packedData |= ((GetUnit1() & 0xFF) << 8);
		packedData |= ((GetUnit2() & 0xFF) << 16);
		packedData |= ((GetActionType() & 0x0F) << 24);
		packedData |= ((GetUnitType() & 0x0F) << 28);

		return packedData;
	}

private:
	unsigned char m_X;
	unsigned char m_Y;
	unsigned int m_Unit1;
	unsigned int m_Unit2;
	ActionType m_ActionType;
	UnitType m_UnitType;
};