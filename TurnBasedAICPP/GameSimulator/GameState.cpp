#include "GameState.h"
#include <algorithm>

bool GameState::IsPassable(int x, int y) const
{
	return std::none_of(m_Units.begin(), m_Units.end(), [x, y](const Unit& unit) {
		return unit.GetX() == x && unit.GetY() == y;
		});
}

bool GameState::IsGameOver() const
{
	//Kings, if they are alive, are always at positions 2-5 in m_Units (P1 King, P1 King, P2 King, P2 King), allowing for this efficient game over check.
	if (m_Units.size() > 4 && m_Units[4].GetUnitType() == King) { return false; }
	if (m_Units.size() > 3 && m_Units[3].GetUnitType() != King) { return true; }
	if (m_Units.size() > 3 && m_Units[2].GetOwner() != m_Units[3].GetOwner()) { return false; }

	return true;
}

int GameState::GetResult() const
{
	//The losing player has no kings present in m_Units, and the winning player has at least one.
	if (IsGameOver())
		return m_Units[2].GetOwner();

	return -1;
}

//Return the player who does NOT make a move this turn
User GameState::GetEnemy() const {
	if (m_PlayerToMove == Player) { return Enemy; }
	if (m_PlayerToMove == Enemy) { return Player; }
	return Neutral;
}

std::vector<Action> GameState::GetLegalMoves() const {

	std::vector<Action> moves;
	addLegalCreateActions(moves);
	
	for (uint32_t i = 2; i < m_Units.size(); i++) {
		const auto& unit = m_Units[i];
		if (!isOwnedByPlayerToMove(unit)) continue;
		
		addLegalMoveActions(moves, unit, i);
		addLegalSwapActions(moves, unit, i);
		addLegalAttackActions(moves, unit, i);
	}
	
	return moves;
}

template<typename Callable>
void GameState::gridLoop(const Callable& functionToExecute) const
{
	for (unsigned char y = 0; y < c_GridSize; y++)
		for (unsigned char x = 0; x < c_GridSize; x++)
			functionToExecute(x, y);
}


void GameState::addLegalCreateActions(std::vector<Action>& moves) const
{
	unsigned char money = GetMoney(m_PlayerToMove);
	if (money < Unit::GetCost(Gobbo)) return;
	
	gridLoop([this, &moves, money](unsigned char x, unsigned char y)
		{
			if (!IsPassable(x, y)) return;

			const std::array<UnitType, 4> units = { Gobbo, Prawn, Building, Knight };

			for (auto unit : units)
			{
				if (money < Unit::GetCost(unit))
					break;

				moves.emplace_back(unit, x, y);
			}
		 });
}

void GameState::addLegalMoveActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const
{
	for (const auto& move : GetMovement(unit))
		moves.emplace_back(unitIndex, move.X, move.Y);
}

void GameState::addLegalAttackActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const
{
	for (const auto& attack : GetAttacks(unit))
		moves.emplace_back(unitIndex, attack);
}

void GameState::addLegalSwapActions(std::vector<Action>& moves, const Unit& unit, uint32_t unitIndex) const
{
	if (unit.GetUnitType() != Prawn) return;

	for (uint32_t swappedUnit = 0; swappedUnit < m_Units.size(); swappedUnit++)
		if (unitIndex != swappedUnit && canSwap(unit, m_Units[swappedUnit]))
			moves.emplace_back(Swap, unitIndex, swappedUnit, 0, 0, Prawn);
}

bool GameState::isOwnedByPlayerToMove(const Unit& unit) const
{
	return unit.GetOwner() == m_PlayerToMove;
}

bool GameState::isResource(const Unit& unit) const
{
	return unit.GetUnitType() == Resource;
}

bool GameState::canSwap(const Unit& unit1, const Unit& unit2) const
{
	return (unit1.GetUnitType() == Prawn) 
		&& (unit1.GetOwner() == unit2.GetOwner())
		&& (unit2.GetOwner() != Neutral)
		&& (!isResource(unit2));
}

std::array<Vec2, 8> GameState::getDirectionVectors(Direction direction) const
{
	const std::array<Vec2, 4> cardinalVectors = { {{1, 0}, {0, 1}, {-1, 0}, {0, -1}} };
	const std::array<Vec2, 4> diagonalVectors = { {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}} };

	std::array<Vec2, 8> vectors;
	std::size_t numVectors = 0;

	switch (direction) {
	case Line:
		std::copy(cardinalVectors.begin(), cardinalVectors.end(), vectors.begin());
		numVectors = cardinalVectors.size();
		break;
	case Diagonal:
		std::copy(diagonalVectors.begin(), diagonalVectors.end(), vectors.begin());
		numVectors = diagonalVectors.size();
		break;
	case LineAndDiagonal:
		numVectors = 0;
		for (const auto& v : cardinalVectors) {
			vectors[numVectors++] = v;
		}
		for (const auto& v : diagonalVectors) {
			vectors[numVectors++] = v;
		}
		break;
	}

	return vectors;
}

std::vector<Pos> GameState::GetMovement(const Unit &unit) const
{
	const int speed = unit.GetSpeed();
	std::vector<Pos> moves;

	if (speed == 0) { return moves; }

	const Pos currentPosition(unit.GetX(), unit.GetY());

	const std::array<Vec2, 8> vectors = getDirectionVectors(unit.GetDirection());

	std::for_each(vectors.begin(), vectors.end(), [this, currentPosition, speed, &moves](const auto& vector) {
		for (int i = 1; i <= speed; ++i) {
			const int x = currentPosition.X + (i * vector.X);
			const int y = currentPosition.Y + (i * vector.Y);

			if (!IsInBounds(x, y) || !IsPassable(x, y))
				break;

			moves.emplace_back(x, y);
		}
		});

	return moves;
}

std::vector<int> GameState::GetAttacks(const Unit &unit) const {

	std::vector<int> enemyUnits;
	std::vector<int> attackActions;

	for (uint32_t i = 2; i < m_Units.size(); i++)
		if (m_Units[i].GetOwner() != Neutral && m_Units[i].GetOwner() != m_PlayerToMove)
			enemyUnits.push_back(i);

	auto vectors = getDirectionVectors(unit.GetDirection());

	for (const auto& vector : vectors)
	{
		for (int i = 1; i <= unit.GetRange(); i++)
		{
			const int x = unit.GetX() + (i * vector.X);
			const int y = unit.GetY() + (i * vector.Y);

			if (!IsInBounds(x, y)) break;

			bool attackMade = false;

			for (auto enemyUnit : enemyUnits)
			{
				if (m_Units[enemyUnit].GetX() == x && m_Units[enemyUnit].GetY() == y)
				{
					attackActions.push_back(enemyUnit);
					attackMade = true;
					break;
				}
			}

			if (attackMade || !IsPassable(x, y)) break;
		}
	}
	return attackActions;
}

std::array<Unit, 4> GameState::GetKings() const
{
	std::array<Unit, 4> kings = { { m_Units[2], m_Units[3], m_Units[4], m_Units[5]} };

	for (int i = 0; i < kings.size(); i++)
		if (kings[i].GetUnitType() != King)
			kings[i] = Unit(15, 15, 0, 0, Neutral);
	return kings;
}

bool GameState::IsInBounds(int x, int y)
{
	if (x < 0
		|| y < 0
		|| y > c_GridSize-1
		|| x > c_GridSize-1
		)
		return false;

	else return true;
}

void GameState::PrintUnits() const
{
	for (auto &unit : m_Units)
	{
		unit.Print();
		std::cout << std::endl;
	}
}

std::array<std::array<char, GameState::c_GridSize>, GameState::c_GridSize> GameState::GetGridRepresentation() const
{
	std::array<std::array<char, c_GridSize>, c_GridSize> grid;
	initialiseGrid(grid);
	placeUnitsOnGrid(grid);
	return grid;
}

void GameState::initialiseGrid(std::array<std::array<char, c_GridSize>, c_GridSize>& grid) const
{
	gridLoop([&grid](unsigned char x, unsigned char y)
		{
			grid[x][y] = '-';
		});
}

void GameState::placeUnitsOnGrid(std::array<std::array<char, c_GridSize>, c_GridSize>& grid) const
{
	for (const auto& unit : m_Units)
	{
		if (!IsInBounds(unit.GetX(), unit.GetY())) continue;
		grid[unit.GetX()][unit.GetY()] = unit.GetCharRepresentation();
	}
}


const Unit& GameState::GetUnit(int index) const
{
	return m_Units[index];
}

void GameState::initDefaultState()
{
	m_Units.emplace_back(15, 15, 8, Resource, Player);
	m_Units.emplace_back(15, 15, 8, Resource, Enemy);
	m_Units.emplace_back(0, 0, 40, King, Player);
	m_Units.emplace_back(0, 14, 40, King, Player);
	m_Units.emplace_back(14, 0, 40, King, Enemy);
	m_Units.emplace_back(14, 14, 40, King, Enemy);

	const std::array<Pos, 8> treePositions =
		{ Pos(7, 1), Pos(1, 7), Pos(13, 7), Pos(7, 13), Pos(6, 6), Pos(8, 8), Pos(6, 8), Pos(8, 6) };

	for (auto& position : treePositions)
		m_Units.emplace_back(position.X, position.Y, 0, Tree, Neutral);
}

unsigned char GameState::GetMoney(User user) const
{
	return (user == (User)m_Units[0].GetOwner()) ? m_Units[0].GetHealth() : m_Units[1].GetHealth();
}

void GameState::PrintData() const
{
	auto moves = GetLegalMoves();

	int attack = 0;
	int move = 0;
	int create = 0;
	int swap = 0;

	for (auto const& possibleMove : moves)
	{
		if (possibleMove.GetActionType() == Attack) { attack++; }
		if (possibleMove.GetActionType() == Create) { create++; }
		if (possibleMove.GetActionType() == Move) { move++; }
		if (possibleMove.GetActionType() == Swap) { swap++; }
	}

	std::cout << "\n\nCurrent State Data\n________________" << std::endl;
	std::cout << "Total actions: " << moves.size() << std::endl;
	std::cout << "\n\nAttack actions: " << attack << "\nMove actions: " << move << "\nCreate actions: " << create << "\nSwap actions: " << swap << std::endl;
	std::cout << "\nCurrent Player Money: " << static_cast<int>(GetMoney(GetPlayer())) << "\nOpponent Money: " << static_cast<int>(GetMoney(GetEnemy())) << std::endl;
	
	//Subtractions exclude resource unit
	int totalUnits = m_Units.size() - 2;
	int friendly = -1;
	int enemy = -1;
	int neutral = 0;

	for (auto const& unit : m_Units)
	{
		if (unit.GetOwner() == Player) friendly++;
		if (unit.GetOwner() == Enemy) enemy++;
		if (unit.GetOwner() == Neutral) neutral++;
	}

	std::cout << "\n\nTotal units: " << totalUnits << "   Non-neutral: " << totalUnits - neutral << std::endl;
	std::cout << "Friendly: " << friendly << "   Enemy: " << enemy << "   Neutral: " << neutral << std::endl;
}