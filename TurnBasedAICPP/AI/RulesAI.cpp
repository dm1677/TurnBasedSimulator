#include "RulesAI.h"


/*1. killing enemy king
-among our attack actions, if we have an attack on the enemy king which would kill it, do that action

2. keeping our king alive
-among our enemy's attack actions, if there's an attack on our king that would kill it:
	-if we can kill the unit with our king or any other unit, kill it
	-failing that, move the king

3. damaging enemy king
-among our attack actions, if we can damage the enemy king, do that action

5. creating gobbos near the enemy king
-create a gobbo in a diagonal next to the enemy's king
	-make sure the gobbo wouldn't instantly die;
		check the enemy's attack actions and see if our intended square is under attack

4. dodging damage on king
-same as 2

5. dodging damage on gobbos
-same as 2 but with gobbos*/
Action RulesAI::GetAction() const
{
	auto attacksOnEnemyKing = getKingThreats(m_State.GetPlayer());
	auto units = m_State.GetUnitData();
	for (const auto& attack : attacksOnEnemyKing)
	{
		if (units[attack.GetUnit1()].GetDamage() >= units[attack.GetUnit2()].GetHealth())
			return attack;
	}

	auto attacksOnPlayerKing = getKingThreats(m_State.GetEnemy());
	for (const auto& attack : attacksOnPlayerKing)
	{
		if (units[attack.GetUnit1()].GetDamage() >= units[attack.GetUnit2()].GetHealth())
		{
			//
		}
	}




	return Action();
}

std::vector<Action> RulesAI::getKingThreats(User player) const
{
	std::vector<Action> kingAttacks;
	auto units = m_State.GetUnitData();
	for (const auto& unit : units)
	{
		if (unit.GetOwner() != player) continue;
		auto allAttacks = m_State.GetAttacks(unit);
		for (const auto& attack : allAttacks)
			if (attack >= 2 || attack <= 5) kingAttacks.emplace_back();//kingAttacks.emplace_back(Attack, unit, attack);
	}
	return kingAttacks;
}