#pragma once
#include "Enums.h"
#include "BitBoard.h"
#include "GameSimulator/Unit.h"
#include "GameSimulator/Action.h"
#include <vector>
#include <array>
#include <iostream>
#include <functional>
#include <cstdint>

struct BBGameState {
public:
	BBGameState() {
		m_PlayerToMove = Player;
	};

	static constexpr unsigned char c_GridSize = 15;

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

	std::array<Unit, 4> GetKings() const;
private:
	BitBoard m_KingBoard[2];
	BitBoard m_GobboBoard[2];
	BitBoard m_PrawnBoard[2];
	BitBoard m_BuildingBoard[2];
	BitBoard m_KnightBoard[2];
	BitBoard m_TreeBoard;
	uint_fast8_t m_HealthBoard[15][15]; //Is there a better/cheaper way to encode this information?
									   //This will have to be updated every move - is that ok?

	//These could be encoded in the extra space in the bitboards
	uint_fast8_t m_Resources[2];
	User m_PlayerToMove;

	BitBoard getPassabilityBoard() const
	{
		return (m_KingBoard[0] | m_KingBoard[1] | m_GobboBoard[0] | m_GobboBoard[1] | m_PrawnBoard[0] | m_PrawnBoard[1]
			| m_BuildingBoard[0] | m_BuildingBoard[1] | m_KnightBoard[0] | m_KnightBoard[1] | m_TreeBoard);
	}

	//---------------------------------------------------------------------------------
	std::array<Vec2, 8> getDirectionVectors(Direction direction) const;
	void addLegalCreateActions(std::vector<Action>& moves) const;
	void addLegalMoveActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const;
	void addLegalAttackActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const;
	void addLegalSwapActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const;
	bool isOwnedByPlayerToMove(const Unit& unit) const;
	bool isResource(const Unit& unit) const;
	bool canSwap(const Unit& unit1, const Unit& unit2) const;
	
	void initialiseGrid(std::array<std::array<char, c_GridSize>, c_GridSize>& grid) const;
	void placeUnitsOnGrid(std::array<std::array<char, c_GridSize>, c_GridSize>& grid) const;

	template<typename Callable>
	void gridLoop(const Callable& logicFunc) const;
};

