#include "TestManager.h"
#include "Test.h"
#include "../AI/MCTSAI.h"
#include "../Serialiser.h"

void TestManager::RunTests() const
{
	Serialiser s;
	const auto puzzles = s.GetPuzzlesFromDirectory("output\\test_puzzles");
	const int num = 10;

	for (const auto& puzzle : puzzles)
	{
		Test test(puzzle, puzzle.GetName());

		std::cout << "Running test: " << puzzle.GetName() << " " << num << " times." << std::endl;

		const AI* ai = new MCTSAI(test.GetState(), false, true, 100, 20);
		auto wrongMoves = test.Run(ai, num);
		delete ai;
	}
}