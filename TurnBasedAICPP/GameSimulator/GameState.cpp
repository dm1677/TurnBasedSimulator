#include "GameState.h"

bool GameState::IsPassable(int x, int y) const
{
	for (Unit unit : m_Units) {
		if (unit.GetX()  == x && unit.GetY() == y) {
			return false;
		}
	}
	return true;
}

bool GameState::IsGameOver() const
{
	if (m_Units[4].GetUnitType() == King) { return false; }
	if (m_Units[3].GetUnitType() != King) { return true; }
	if (m_Units[2].GetOwner() != m_Units[3].GetOwner()) { return false; }

	return true;
}

int GameState::GetResult() const
{
	if (IsGameOver())
		return m_Units[2].GetOwner();

	return -1;
}

User GameState::GetEnemy() const {
	if (m_PlayerToMove == Player) { return Enemy; }
	if (m_PlayerToMove == Enemy) { return Player; }
}

std::vector<Action> GameState::GetLegalMoves() const {

	std::vector<Action> moves;
	unsigned char money = GetMoney(m_PlayerToMove);
	
	if (money >= 3)
	{
		for (unsigned char  y = 0; y < 15; y++)
		{
			for (unsigned char x = 0; x < 15; x++)
			{
				if (!IsPassable(x, y)) continue;

				moves.emplace_back(Gobbo, x, y);
				if (money >= 5)
					moves.emplace_back(Prawn, x, y);
				if (money >= 8)
					moves.emplace_back( Building, x, y );
				if (money >= 12)
					moves.emplace_back(Knight, x, y );
			}
		}
	}
	
	for (uint32_t i = 0; i < m_Units.size(); i++) {
		if (m_Units[i].GetOwner() != (int)m_PlayerToMove) continue;
		if (m_Units[i].GetUnitType() == (int)Resource) continue;
		
		for (const auto &move : GetMovement(m_Units[i]))
			moves.emplace_back(i, move.X, move.Y);

		for (auto &attack : GetAttacks(m_Units[i]))
			moves.emplace_back(i, attack);
	}
	
	return moves;
}

std::vector<Pos> GameState::GetMovement(const Unit &unit) const
{
	int speed = unit.GetSpeed();
	std::vector<Pos> moves;

	if (speed == 0) { return moves; }

	Pos currentPosition(unit.GetX(), unit.GetY());

	const std::vector<Pos> cardinalVectors = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	const std::vector<Pos> diagonalVectors = { { 1, -1 }, { 1, 1 }, { -1, 1 }, { -1, -1 } };

	std::vector<Pos> vectors;

	switch (unit.GetDirection()) {
	case Line:
		vectors = cardinalVectors;
		break;
	case Diagonal:
		vectors = diagonalVectors;
		break;
	case LineAndDiagonal:
		vectors = cardinalVectors;
		vectors.insert(vectors.end(), diagonalVectors.begin(), diagonalVectors.end());
		break;
	}

	int x, y;
	for (auto k = 0; k < vectors.size(); k++)
	{
		for (auto i = 1; i <= speed; i++)
		{
			x = currentPosition.X + (i * vectors[k].X);
			y = currentPosition.Y + (i * vectors[k].Y);

			if (!IsInBounds(x, y) || !IsPassable(x, y))
				break;

			moves.emplace_back(x, y);
		}
	}

	return moves;
}

std::vector<int> GameState::GetAttacks(const Unit &unit) const {

	std::vector<int> enemyUnits;
	std::vector<int> attackActions;

	for (uint32_t i = 2; i < m_Units.size(); i++)
	{
		if (m_Units[i].GetOwner() != Neutral && m_Units[i].GetOwner() != m_PlayerToMove)
		{
			enemyUnits.push_back(i);
		}
	}

	const std::vector<Pos> cardinal_vectors = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	const std::vector<Pos> diagonal_vectors = { { 1, -1 }, { 1, 1 }, { -1, 1 }, { -1, -1 } };

	std::vector<Pos> vectors;

	switch (unit.GetDirection()) {
	case Line:
		vectors = cardinal_vectors;
		break;
	case Diagonal:
		vectors = diagonal_vectors;
		break;
	case LineAndDiagonal:
		vectors = cardinal_vectors;
		vectors.insert(vectors.end(), diagonal_vectors.begin(), diagonal_vectors.end());
		break;
	}
	int x, y;

	for (uint32_t k = 0; k < vectors.size(); k++)
	{
		for (int i = 1; i <= unit.GetRange(); i++)
		{
			x = unit.GetX() + (i * vectors[k].X);
			y = unit.GetY() + (i * vectors[k].Y);

			if (!IsInBounds(x, y) || !IsPassable(x, y)) break;

			for (uint32_t j = 1; j < enemyUnits.size(); j++)
			{
				if (m_Units[j].GetX() == x && m_Units[j].GetY() == y) {
					attackActions.push_back(j);
				}
			}
		}
	}

	return attackActions;
}

bool GameState::IsInBounds(int x, int y)
{
	if (x < 0
		|| y < 0
		|| y > 14
		|| x > 14
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

void GameState::DrawGrid() const
{
	char grid[15][15];

	std::cout << std::endl;

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			grid[x][y] = '-';
		}
	}

	for (auto &unit : m_Units)
	{
		grid[unit.GetX()][unit.GetY()] = unit.GetCharRepresentation();
	}

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			std::cout << grid[x][y];
		}
		std::cout << std::endl;
	}
}

void GameState::createUnits()
{
	m_Units.emplace_back(15, 15, 8, Resource, Player);
	m_Units.emplace_back(15, 15, 8, Resource, Enemy);
	m_Units.emplace_back(0, 0, 40, King, Player);
	m_Units.emplace_back(0, 14, 40, King, Player);
	m_Units.emplace_back(14, 0, 40, King, Enemy);
	m_Units.emplace_back(14, 14, 40, King, Enemy);

	const std::vector<Pos> treePositions = 
		{ { 7, 0 }, { 0, 7 }, { 14, 7 }, { 7, 14 }, { 6,6 }, { 8,8 }, { 6,8 }, { 8,6 } };

	for (auto& position : treePositions)
		m_Units.emplace_back(position.X, position.Y, 0, Tree, Neutral);
}

unsigned char GameState::GetMoney(User user) const
{
	return (user == (User)m_Units[0].GetOwner()) ? m_Units[0].GetHealth() : m_Units[1].GetHealth();
}

void GameState::GetMoveCounts() const
{
	auto moves = GetLegalMoves();
	std::cout << "Total actions: " << "\n\n" << moves.size();

	int attack = 0;
	int move = 0;
	int create = 0;

	for (auto const& possibleMove : moves) {
		if (possibleMove.GetActionType() == Attack) { attack++; }
		if (possibleMove.GetActionType() == Create) { create++; }
		if (possibleMove.GetActionType() == Move) { move++; }
	}

	std::cout << "\n\nAttack actions: " << attack << "\nMove actions: " << move << "\nCreate actions: " << create;
	std::cout << "\nMoney: " << GetMoney(GetPlayer()) << std::endl;
}