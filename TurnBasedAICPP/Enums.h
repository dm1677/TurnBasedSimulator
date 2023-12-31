#pragma once

namespace TurnBasedSimulator
{
	enum UnitType
	{
		Prawn,
		Building,
		King,
		Knight,
		Gobbo,
		Tree,
		Resource
	};

	enum User {
		Player,
		Enemy,
		Neutral
	};

	enum ActionType {
		Move,
		Attack,
		Create,
		Swap
	};

	enum Direction {
		Line,
		Diagonal,
		LineAndDiagonal
	};

	enum FileType {
		Binary,
		Text
	};

	enum class ActionCategory
	{
		None,
		Create,
		Attack,
		Move,
		Swap,
		AttackOnKing,
		KingAttack,
		KingMove,
		ThreatOnKing,
		KingEscape,
		MoveIntoDamage
	};


	struct Pos
	{
	public:
		Pos(unsigned char x, unsigned char y) : X(x), Y(y) {}
		const unsigned char X, Y;
	};

	struct Vec2
	{
	public:
		Vec2() : X(0), Y(0) {}
		Vec2(int x, int y) : X(x), Y(y) {}
		int X, Y;
	};
}