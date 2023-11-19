#pragma once
#include <chrono>
#include "../GameSimulator/GameState.h"
#include "../AI/AI.h"
#include "../Puzzle.h"

class Test
{
public:
	explicit Test(const Puzzle& puzzle, const std::string& name = "Test")
		: m_Puzzle(puzzle), m_Name(name) {}

	const GameState& GetState() const { return m_Puzzle.GetState(); }
	std::vector<Action> Run(const AI* ai, int attempts = 1, bool cumulative = false);
	void PrintInfo() const;

private:
	Puzzle m_Puzzle;
	std::string m_Name;

	int m_Attempts = 0;
	int m_Successes = 0;
	long long m_Duration = 0;
};