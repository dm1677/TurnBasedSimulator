#pragma once
#include "Enums.h"
#include "Unit.h"
#include "Action.h"
#include <vector>
#include <iostream>
#include <stack>

struct GameState {
public:
	GameState() {
		createUnits();
		m_PlayerToMove = Player;
	};
	explicit GameState(const std::vector<Unit>& units, User player) : m_Units(units), m_PlayerToMove(player) {};

	User GetPlayer() const { return m_PlayerToMove; }
	User GetEnemy() const;

	unsigned char GetMoney(User user) const;

	static bool IsInBounds(int x, int y);
	bool IsPassable(int x, int y) const;
	bool IsGameOver();
	int GetResult();

	std::vector<Action> GetLegalMoves();
	std::vector<std::pair<unsigned char, unsigned char>> GetMovement(const Unit &unit) const;
	std::vector<int> GetAttacks(const Unit &unit);

	void PrintUnits();
	void DrawGrid();

	std::vector<Unit> GetUnitData() const { return m_Units; }
private:
	std::vector<Unit> m_Units;
	std::stack<Action> m_Actions;
	User m_PlayerToMove;

	void createUnits();
};

