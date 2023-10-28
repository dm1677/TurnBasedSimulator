#pragma once
#include "../Enums.h"
#include "Unit.h"
#include "Action.h"
#include <vector>
#include <array>
#include <iostream>
#include <functional>

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
	bool IsGameOver() const;
	int GetResult() const;

	std::vector<Action> GetLegalMoves() const;
	std::vector<Pos> GetMovement(const Unit &unit) const;
	std::vector<int> GetAttacks(const Unit &unit) const;

	void GetMoveCounts() const;
	void PrintUnits() const;
	void DrawGrid() const;

	std::vector<Unit> GetUnitData() const { return m_Units; }
private:
	std::vector<Unit> m_Units;
	User m_PlayerToMove;

	void createUnits();
	std::array<Vec2, 8> getDirectionVectors(Direction direction) const;
	void addLegalCreateActions(std::vector<Action>& moves) const;
	void addLegalMoveActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const;
	void addLegalAttackActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const;
	void addLegalSwapActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const;
	bool isOwnedByPlayerToMove(const Unit& unit) const;
	bool isResource(const Unit& unit) const;
	bool canSwap(const Unit& unit1, const Unit& unit2) const;

	using LogicFunction = std::function<void(unsigned char x, unsigned char y, std::vector<Action>& moves)>;
	void gridLoop(std::vector<Action>& moves, LogicFunction logicFunc) const;
};

