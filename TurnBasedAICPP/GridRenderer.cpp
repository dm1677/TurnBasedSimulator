#include "GridRenderer.h"

void GridRenderer::DrawGrid() const
{
	HANDLE hConsole = getConsoleHandle();
	auto grid = m_State.GetGridRepresentation();

	displayGrid(grid, hConsole);
	SetConsoleTextAttribute(hConsole, c_ForegroundColour);
}

HANDLE GridRenderer::getConsoleHandle() const
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

void GridRenderer::displayGrid(const std::array<std::array<char, GameState::c_GridSize>, GameState::c_GridSize>& grid, HANDLE hConsole) const
{
	std::cout << std::endl;

	for (int y = 0; y < GameState::c_GridSize; y++)
	{
		for (int x = 0; x < GameState::c_GridSize; x++)
		{
			char unitChar = grid[x][y];
			setUnitColour(hConsole, x, y, unitChar);
			std::cout << unitChar;
		}
		std::cout << std::endl;
	}
}

void GridRenderer::setUnitColour(HANDLE hConsole, int x, int y, char unitChar) const
{
	if (unitChar != '-')
	{
		//TODO: Change such that no copy is made
		auto units = m_State.GetUnitData();
		const auto& unit = *std::find_if(units.begin(), units.end(),
			[x, y](const Unit& u) { return u.GetX() == x && u.GetY() == y; });

		SetConsoleTextAttribute(hConsole, getUnitcolour(unit));
	}
	else
	{
		SetConsoleTextAttribute(hConsole, c_ForegroundColour);
	}
}

WORD GridRenderer::getUnitcolour(const Unit& unit) const
{
	switch (unit.GetOwner())
	{
	case Player:
		return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	case Enemy:
		return FOREGROUND_RED | FOREGROUND_INTENSITY;
	case Neutral:
		return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	default:
		return c_ForegroundColour;
	}
}