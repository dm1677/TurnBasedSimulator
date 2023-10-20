#pragma once
#include "Enums.h"

struct Unit
{
public:
	Unit(unsigned char x, unsigned char y, unsigned char health, unsigned char unit_type, unsigned char owner) 
		: m_X(x), m_Y(y), m_Health(health), m_UnitType(unit_type), m_Owner(owner) {}

	unsigned char GetX() const { return m_X; }
	unsigned char GetY() const { return m_Y; }
	unsigned char GetHealth() const { return m_Health; }

	int GetUnitType() const { return m_UnitType; }
	int GetOwner() const { return m_Owner; }

	void SetX(unsigned char newX) { m_X = newX; }
	void SetY(unsigned char newY) { m_Y = newY; }
	void SetHealth(unsigned char newHealth) { m_Health = newHealth; }

	bool TakeDamage(unsigned char damage);

	Direction GetDirection() const;
	static Direction GetDirection(UnitType unit_type);

	int GetSpeed() const;
	static int GetSpeed(UnitType unit_type);

	int GetMaxHealth() const;
	static int GetMaxHealth(UnitType unit_type);

	unsigned char GetDamage() const;
	static unsigned char GetDamage(UnitType unit_type);

	unsigned char GetCost() const;
	static unsigned char GetCost(UnitType unit_type);

	int GetRange() const;
	static int GetRange(UnitType unit_type);

	bool IsMoveAttacker() const;
	static bool IsMoveAttacker(UnitType unit_type);

	char GetCharRepresentation() const;

	void Print() const;

private:
	unsigned char m_X : 4;
	unsigned char m_Y : 4;
	unsigned char m_Health : 8;
	unsigned char m_UnitType : 3;
	unsigned char m_Owner : 2;
};

