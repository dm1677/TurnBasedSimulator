#pragma once
#include "Enums.h"

struct Action {

public:

	unsigned char GetX() const { return x; }
	unsigned char GetY() const { return y; }
	int GetUnit1() const { return unit1; }
	int GetUnit2() const { return unit2; }
	ActionType GetActionType() const { return action_type; }
	UnitType GetUnitType() const { return unit_type; }

	

	//Move unit to x, y
	Action(int unit, int x, int y) :action_type(Move), unit1(unit), x(x), y(y) {}

	//Create unit_type at x, y
	Action(UnitType unit_type, int x, int y) :action_type(Create), unit_type(unit_type), x(x), y(y) {}

	//Attack defender with attacker
	Action(int attacker, int defender) :action_type(Attack), unit1(attacker), unit2(defender) {}

	Action() {};

private:
	unsigned char x;
	unsigned char y;
	int unit1;
	int unit2;
	ActionType action_type;
	UnitType unit_type;
};