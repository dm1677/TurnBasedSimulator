#include "Test.h"

void Test::Run(const AI* ai, int attempts, bool cumulative)
{
	if (!cumulative)
	{
		m_Successes = 0;
		m_Attempts = 0;
		m_Duration = 0;
	}
	
	for (int i = 0; i < attempts; i++)
		run(ai);
}

void Test::PrintInfo() const
{
	std::cout << m_Successes << "/" << m_Attempts << " tests succeeded." << std::endl
		<< 100 * (double)m_Successes / (double)m_Attempts << "% of tests passed." << std::endl
		<< "Completed in " << m_Duration << "ms. Average: " << (double)m_Duration / (double)m_Attempts << "ms per test." << std::endl;
}

void Test::run(const AI* ai)
{
	auto start = std::chrono::high_resolution_clock::now();

	m_Attempts++;
	if (auto action = ai->GetAction(); m_Puzzle.IsCorrectAction(action))
		m_Successes++;
	else
		m_WrongMoves.push_back(action);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	m_Duration += duration.count();
}
