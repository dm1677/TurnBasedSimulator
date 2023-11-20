#include "BBGameState.h"

User BBGameState::GetEnemy() const
{
	if (m_PlayerToMove == Player) return Enemy;
	if (m_PlayerToMove == Enemy) return Player;
}

unsigned char BBGameState::GetMoney(User user) const
{
	return (unsigned char)m_Resources[user];
}

bool BBGameState::IsInBounds(int x, int y)
{
	if (x < 0
		|| y < 0
		|| y > c_GridSize - 1
		|| x > c_GridSize - 1
		)
		return false;

	else return true;
}

bool BBGameState::IsPassable(int x, int y) const
{
	const auto& allUnits = getPassabilityBoard();
	return allUnits.IsSet(x, y);
}

bool BBGameState::IsGameOver() const
{
	return (m_KingBoard[0].IsEmpty() || m_KingBoard[1].IsEmpty());
}

int BBGameState::GetResult() const
{
	if (m_KingBoard[0].IsEmpty()) return 1;
	if (m_KingBoard[1].IsEmpty()) return 0;
}

std::vector<Action> BBGameState::GetLegalMoves() const
{
	
	
	
	return std::vector<Action>();
}
