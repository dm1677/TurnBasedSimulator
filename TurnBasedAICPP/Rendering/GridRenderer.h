#pragma once
#include <array>
#include <Windows.h>
#include "../GameSimulator/Unit.h"
#include "../GameSimulator/GameState.h"

class GridRenderer
{
public:
	explicit GridRenderer(const GameState& state)
		: m_State(state) {}

	void DrawGrid() const;
private:
	const WORD c_ForegroundColour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	HANDLE getConsoleHandle() const;
	void displayGrid(const std::array<std::array<char, GameState::c_GridSize>, GameState::c_GridSize>& grid, HANDLE hConsole) const;
	void setUnitColour(HANDLE hConsole, int x, int y, char unitChar) const;
	WORD getUnitcolour(const Unit& unit) const;

	const GameState m_State;
};

