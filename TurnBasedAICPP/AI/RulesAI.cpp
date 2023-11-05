#include "RulesAI.h"
#include "../GameSimulator/Simulator.h"


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
	auto attacksOnEnemyKing = getKingThreats(m_State.GetPlayer(), m_State);
	auto& units = m_State.GetUnitData();
	for (const auto& attack : attacksOnEnemyKing)
	{
		if (units[attack.GetUnit1()].GetDamage() >= units[attack.GetUnit2()].GetHealth())
			return attack;
	}

	auto friendlyAttacks = std::vector<Action>();
	for (int unit = 2; unit < units.size(); unit++)
	{
		if (units[unit].GetOwner() == m_State.GetPlayer())
		{
			const auto& attacks = m_State.GetAttacks(units[unit]);
			for (const auto attackedUnit : attacks)
				friendlyAttacks.emplace_back(Attack, unit, attackedUnit, 0, 0, Prawn);
		}
	}
	
	const auto& attacksOnPlayerKing = getKingThreats(m_State.GetEnemy(), m_State);
	for (const auto& attack : attacksOnPlayerKing)
	{
		const auto& enemyUnit = units[attack.GetUnit1()];
		const auto& friendlyKing = units[attack.GetUnit2()];
		if (enemyUnit.GetDamage() >= friendlyKing.GetHealth())
		{
			for (const auto& friendlyAttack : friendlyAttacks)
			{
				const auto& friendlyUnit = units[friendlyAttack.GetUnit1()];
				if ((attack.GetUnit1() == friendlyAttack.GetUnit2())
					&& (enemyUnit.GetHealth() <= friendlyUnit.GetDamage()))
					return friendlyAttack;
			}
			const auto& moves = m_State.GetMovement(friendlyKing);
			auto moveActions = std::vector<Action>();
			moveActions.reserve(moves.size());
			for (const auto& move : moves)
				moveActions.emplace_back(attack.GetUnit2(), move.X, move.Y);
			auto safeMove = getSafestMove(moveActions);
			if (safeMove.GetActionType() == Move) return safeMove;
		}
	}

	int max = 0;
	Action a = Action(Swap, 0, 0, 0, 0, Prawn);
	Action& best = a;
	for (const auto& attack : attacksOnEnemyKing)
	{
		if (units[attack.GetUnit1()].GetDamage() > max)
		{
			max = units[attack.GetUnit1()].GetDamage();
			best = attack;
		}
	}
	if (best.GetActionType() == Attack) return best;

	
	const std::array<Vec2, 4> diagonalVectors = { {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}} };
	for (int i = 2; i <= 5; i++)
	{
		const auto& enemyKing = units[i];
		if (enemyKing.GetOwner() == m_State.GetEnemy())
		{
			if (m_State.GetMoney(m_State.GetPlayer()) >= Unit::GetCost(Gobbo))
			{
				for (const auto& vector : diagonalVectors)
				{
					const int x = enemyKing.GetX() + vector.X;
					const int y = enemyKing.GetY() + vector.Y;
					if (GameState::IsInBounds(x, y) && m_State.IsPassable(x, y))
						return Action(Gobbo, x, y);
				}
			}
	
			else
			{
				auto friendlyGobbos = std::vector<int>();
				for (int k = 2; k < units.size(); k++)
				{
					const auto& unit = units[k];
					if (unit.GetUnitType() == Gobbo && unit.GetOwner() == m_State.GetPlayer())
						friendlyGobbos.push_back(k);
				}
				for (const auto& vector : diagonalVectors)
				{
					const int x = enemyKing.GetX() + vector.X;
					const int y = enemyKing.GetY() + vector.Y;
					for (const auto gobbo : friendlyGobbos)
					{
						const auto& gobboMoves = m_State.GetMovement(units[gobbo]);
						for (const auto& gobboMove : gobboMoves)
						{
							if (gobboMove.X == x && gobboMove.Y == y)
								return Action(gobbo, x, y);
						}
					}
				}
			}
		}
	}
	return Action();
}

std::vector<Action> RulesAI::getKingThreats(User player, const GameState& state) const
{
	std::vector<Action> kingAttacks;
	auto& units = state.GetUnitData();
	for (int i = 2; i < units.size(); i++)
	{
		if (units[i].GetOwner() != player) continue;
		auto allAttacks = state.GetAttacks(units[i]);
		for (const auto& attack : allAttacks)
			if (attack >= 2 || attack <= 5) kingAttacks.emplace_back(Attack, i, attack);
	}
	return kingAttacks;
}

Action RulesAI::getSafestMove(const std::vector<Action>& actions) const
{
	for (const auto& action : actions)
		//if (getKingThreats(m_State.GetPlayer(), Simulator(m_State, action).GetCurrentState()).empty()) return action;
	return Action(Swap, 0, 0, 0, 0, Prawn);
}