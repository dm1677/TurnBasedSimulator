#include "RandomAI.h"
#include <random>
#include <map>

Action RandomAI::GetAction() const
{
	if (m_UseHeuristic) return getSemirandomAction();
	auto actions = m_State.GetLegalMoves();
	
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, actions.size()-1);
	
	int randomNumber = dist(rng);

	if (actions.empty()) return Action();
	return actions[randomNumber];
}

bool RandomAI::isUnitThreatened(const Unit& unit) const
{
	std::array<UnitType, 5> units = { Gobbo, Prawn, Building, Knight, King };
	for (const auto unitType : units)
	{
		//Cheaply simulate whether we're under attack by creating a temporary unit at the same position as our unit
		//and then check whether it attacks its own type
		auto tempUnit = Unit(unit.GetX(), unit.GetY(), 1, unitType, m_State.GetPlayer());
		for (const auto attack : m_State.GetAttacks(tempUnit))
		{
			if (m_State.GetUnit(attack).GetUnitType() == unitType)
				return true;
		}
	}
	return false;
}

//Return whether or not a unit at position, of type unitType, owned by the current player would threaten any enemy kings.
bool RandomAI::wouldThreatenKing(Pos position, UnitType unitType) const
{
	//Cheaply simulate a Create Action and check whether it would threaten a king owned by kingOwner.
	auto tempUnit = Unit(position.X,
		                 position.Y,
		                 1,
		                 unitType,
		                 m_State.GetPlayer());

	auto attacks = m_State.GetAttacks(tempUnit);
	return std::any_of(attacks.begin(),
		               attacks.end(), 
		               [this](int id) 
		               { 
					        return m_State.GetUnit(id).GetUnitType() == King 
								&& m_State.GetPlayer() != m_State.GetUnit(id).GetOwner();
		               });
}

ActionCategory RandomAI::categoriseAction(const Action& action) const
{
	const auto& unit = m_State.GetUnit(action.GetUnit1());
	switch (action.GetActionType())
	{
	case Attack:
		if (m_State.GetUnit(action.GetUnit2()).GetUnitType() == King)
			return ActionCategory::AttackOnKing;
		else if (m_State.GetUnit(action.GetUnit1()).GetUnitType() == King)
			return ActionCategory::KingAttack;
		else
			return ActionCategory::Attack;
		break;
	case Create:
		if (wouldThreatenKing(Pos(action.GetX(), action.GetY()), action.GetUnitType()))
			return ActionCategory::ThreatOnKing;
		else
			return ActionCategory::Create;
		break;		
	case Move:
		if (unit.GetUnitType() == King && isUnitThreatened(unit))
			return ActionCategory::KingEscape;
		else
		{
			if (wouldThreatenKing(Pos(action.GetX(), action.GetY()), (UnitType)m_State.GetUnit(action.GetUnit1()).GetUnitType()))
				return ActionCategory::ThreatOnKing;
			auto tempUnit = Unit(action.GetX(), action.GetY(), 1, unit.GetUnitType(), m_State.GetPlayer());
			if (isUnitThreatened(tempUnit))
				return ActionCategory::MoveIntoDamage; //NOTE: This is not necessarily disjoint with other categories, so it's based on order here.
			else
				return ActionCategory::Move;
		}
		break;		
	case Swap:
		const auto& swappedUnit = m_State.GetUnit(action.GetUnit2());
		if (swappedUnit.GetUnitType() == King && isUnitThreatened(swappedUnit))
			return ActionCategory::KingEscape;
		else
		{
			if (wouldThreatenKing(Pos(m_State.GetUnit(action.GetUnit2()).GetX(), m_State.GetUnit(action.GetUnit2()).GetY()), (UnitType)m_State.GetUnit(action.GetUnit2()).GetUnitType()))
				return ActionCategory::ThreatOnKing;
			else
				return ActionCategory::Swap;
		}
		break;	
	}
}

Action RandomAI::getSemirandomAction() const
{
	std::map<ActionCategory, double> c_Weights =
	{
		{ActionCategory::Create, 1},
		{ActionCategory::Move, 60},
		{ActionCategory::Attack, 400},
		{ActionCategory::Swap, 40},
		{ActionCategory::AttackOnKing, 5500},
		{ActionCategory::KingMove, 120},
		{ActionCategory::KingAttack, 200},
		{ActionCategory::ThreatOnKing, 800},
		{ActionCategory::KingEscape, 4500},
		{ActionCategory::MoveIntoDamage, 10},
		{ActionCategory::None, 0}
	};

	auto actions = m_State.GetLegalMoves();

	std::vector<double> c_DistributionWeights;
	for (const auto& action : actions)
	{
		ActionCategory category = categoriseAction(action);
		c_DistributionWeights.push_back(c_Weights[category]);
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> distribution(c_DistributionWeights.begin(), c_DistributionWeights.end());

	if (actions.empty()) return Action();
	return actions[distribution(gen)];
}
