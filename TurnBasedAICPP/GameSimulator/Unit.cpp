#include "Unit.h"
#include <iostream>

namespace TurnBasedSimulator
{
	//Returns true if the unit's health would be reduced to a value below 0, otherwise applies the damage and returns false.
	bool Unit::TakeDamage(unsigned char damage)
	{
		if (m_Health > damage)
		{
			m_Health -= damage;
			return false;
		}
		return true;
	}

	bool Unit::IsMoveAttacker(UnitType unitType)
	{
		switch (unitType) {
		case (int)Prawn:
			return true;
		case (int)Building:
			return false;
		case (int)King:
			return false;
		case (int)Knight:
			return true;
		case (int)Gobbo:
			return true;
		default:
			return Line;
		}
	}

	char Unit::GetCharRepresentation(UnitType unitType)
	{
		switch (unitType) {
		case (int)Prawn:
			return 'P';
		case (int)Building:
			return '+';
		case (int)King:
			return 'K';
		case (int)Knight:
			return 'T';
		case (int)Gobbo:
			return 'G';
		case (int)Tree:
			return 'Y';
		case (int)Resource:
			return '$';
		default:
			return 0;
		}
	}

	Direction Unit::GetDirection(UnitType unitType)
	{
		switch (unitType) {
		case (int)Prawn:
			return Line;
		case (int)Building:
			return Line;
		case (int)King:
			return Line;
		case (int)Knight:
			return LineAndDiagonal;
		case (int)Gobbo:
			return Diagonal;
		default:
			return Line;
		}
	}

	unsigned char Unit::GetSpeed(UnitType unitType) {
		switch (unitType) {
		case (int)Prawn:
			return 1;
		case (int)Building:
			return 0;
		case (int)King:
			return 2;
		case (int)Knight:
			return 1;
		case (int)Gobbo:
			return 3;
		case (int)Tree:
			return 0;
		case (int)Resource:
			return 0;
		default:
			return 0;
		}
	}

	unsigned char Unit::GetMaxHealth(UnitType unitType) {
		switch (unitType) {
		case (int)Prawn:
			return 5;
		case (int)Building:
			return 15;
		case (int)King:
			return 40;
		case (int)Knight:
			return 15;
		case (int)Gobbo:
			return 5;
		case (int)Tree:
			return 0;
		case (int)Resource:
			return 0;
		default:
			return 0;
		}
	}

	unsigned char Unit::GetDamage(UnitType unitType) {
		switch (unitType) {
		case (int)Prawn:
			return 5;
		case (int)Building:
			return 10;
		case (int)King:
			return 7;
		case (int)Knight:
			return 12;
		case (int)Gobbo:
			return 5;
		case (int)Tree:
			return 0;
		case (int)Resource:
			return 0;
		default:
			return 0;
		}
	}

	unsigned char Unit::GetCost(UnitType unitType) {
		switch (unitType) {
		case (int)Prawn:
			return 5;
		case (int)Building:
			return 8;
		case (int)King:
			return 0;
		case (int)Knight:
			return 12;
		case (int)Gobbo:
			return 3;
		case (int)Tree:
			return 0;
		case (int)Resource:
			return 0;
		default:
			return 0;
		}
	}

	int Unit::GetRange(UnitType unitType)
	{
		switch (unitType) {
		case (int)Prawn:
			return 2;
		case (int)Building:
			return 5;
		case (int)King:
			return 3;
		case (int)Knight:
			return 1;
		case (int)Gobbo:
			return 1;
		default:
			return 0;
		}
	}

	uint32_t Unit::ToBinary() const
	{
		uint32_t packedData = 0;

		packedData |= (GetX() & 0x0F);
		packedData |= ((GetY() & 0x0F) << 4);
		packedData |= ((GetHealth() & 0xFF) << 8);
		packedData |= ((GetUnitType() & 0x07) << 16);
		packedData |= ((GetOwner() & 0x03) << 19);

		return packedData;
	}
}