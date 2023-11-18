#pragma once
#include "../Enums.h"
#include <cstdint>

struct Unit
{
public:
	Unit(unsigned char x, unsigned char y, unsigned char health, unsigned char unitType, unsigned char owner) 
		: m_X(x), m_Y(y), m_Health(health), m_UnitType(unitType), m_Owner(owner) {}

	explicit Unit(uint32_t packedData)
		: m_X(packedData & 0xFF), m_Y((packedData >> 4) & 0x0F), m_Health((packedData >> 8) & 0xFF), m_UnitType((packedData >> 16) & 0x07), m_Owner((packedData >> 19) & 0x03) {}

	unsigned char GetX() const { return m_X; }
	unsigned char GetY() const { return m_Y; }
	unsigned char GetHealth() const { return m_Health; }
	int GetUnitType() const { return m_UnitType; }
	int GetOwner() const { return m_Owner; }

	void SetX(unsigned char newX) { m_X = newX; }
	void SetY(unsigned char newY) { m_Y = newY; }
	void SetHealth(unsigned char newHealth) { m_Health = newHealth; }

	bool TakeDamage(unsigned char damage);
	void Kill() { m_X = 15; m_Y = 15; m_Health = 0; m_Owner = Neutral; }

	static Direction GetDirection(UnitType unitType);
	static unsigned char GetSpeed(UnitType unitType);
	static unsigned char GetMaxHealth(UnitType unitType);
	static unsigned char GetDamage(UnitType unitType);
	static unsigned char GetCost(UnitType unitType);
	static int GetRange(UnitType unitType);
	static bool IsMoveAttacker(UnitType unitType);
	static char GetCharRepresentation(UnitType unitType);

	Direction GetDirection() const { return GetDirection((UnitType)m_UnitType); }
	unsigned char GetSpeed() const { return GetSpeed((UnitType)m_UnitType); }
	unsigned char GetMaxHealth() const { return GetMaxHealth((UnitType)m_UnitType); }
	unsigned char GetDamage() const { return GetDamage((UnitType)m_UnitType); }
	unsigned char GetCost() const { return GetCost((UnitType)m_UnitType); }
	int GetRange() const { return GetRange((UnitType)m_UnitType); }
	bool IsMoveAttacker() const { return IsMoveAttacker((UnitType)m_UnitType); }
	char GetCharRepresentation() const { return GetCharRepresentation((UnitType)m_UnitType); }

	uint32_t ToBinary() const;
private:
	unsigned char m_X : 4;
	unsigned char m_Y : 4;
	unsigned char m_Health : 8;
	unsigned char m_UnitType : 3;
	unsigned char m_Owner : 2;
};

