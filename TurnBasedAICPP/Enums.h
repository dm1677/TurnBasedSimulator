#pragma once

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
	Create
};

enum Direction {
	Line,
	Diagonal,
	LineAndDiagonal
};