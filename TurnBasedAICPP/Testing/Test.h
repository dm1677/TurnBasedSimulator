#pragma once
#include <chrono>
#include "../GameSimulator/GameState.h"
#include "../AI/AI.h"
#include "../Puzzle.h"

namespace TurnBasedSimulator
{
	class Test
	{
	public:
		explicit Test(const Puzzle& puzzle, const std::string& name = "Test")
			: m_Puzzle(puzzle), m_Name(name) {}

		const GameState& GetState() const { return m_Puzzle.GetState(); }
		void Run(const AI* ai, int attempts = 1, bool cumulative = false);
		void PrintInfo() const;
		const std::vector<Action>& GetWrongMoves() const { return m_WrongMoves; }

	private:
		Puzzle m_Puzzle;
		std::string m_Name;

		std::vector<Action> m_WrongMoves;
		int m_Attempts = 0;
		int m_Successes = 0;
		long long m_Duration = 0;

		void run(const AI* ai);
	};
}