#pragma once
#include "Enums.h"

struct Unit
{
public:
	Unit(int x, int y, int health, int unit_type, int owner) :x(x), y(y), health(health), unit_type(unit_type), owner(owner) {}

	unsigned char GetX() const { return x; }
	unsigned char GetY() const { return y; }
	int getHealth() const { return health; }

	int getUnitType() const { return unit_type; }
	int getOwner() const { return owner; }

	void setX(unsigned char newX) { x = newX; }
	void setY(unsigned char newY) { y = newY; }
	void setHealth(unsigned char newHealth) { health = newHealth; }

	bool TakeDamage(unsigned char damage);

	Direction get_direction() const;
	static Direction get_direction(UnitType unit_type);

	int get_speed() const;
	static int get_speed(UnitType unit_type);

	int GetMaxHealth() const;
	static int GetMaxHealth(UnitType unit_type);

	unsigned char GetDamage() const;
	static unsigned char GetDamage(UnitType unit_type);

	int get_cost() const;
	static int get_cost(UnitType unit_type);

	int get_range() const;
	static int get_range(UnitType unit_type);

	bool IsMoveAttacker() const;
	static bool IsMoveAttacker(UnitType unit_type);

	char get_char_representation();

	void print();
	void write();
	void read();

private:
	unsigned char x : 4;
	unsigned char y : 4;
	unsigned char health : 8;
	unsigned char unit_type : 3;
	unsigned char owner : 2;
};

