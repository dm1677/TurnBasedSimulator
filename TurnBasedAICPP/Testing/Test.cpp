#include "Test.h"

std::vector<Action> Test::Run(const AI* ai, int attempts = 1, bool cumulative = false)
{
	if (!cumulative)
	{
		m_Successes = 0;
		m_Attempts = 0;
		m_Duration = 0;
	}
	auto failedActions = std::vector<Action>();

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < attempts; i++)
	{
		m_Attempts++;
		auto action = ai->GetAction();
		if (m_Puzzle.IsCorrectAction(action))
			m_Successes++;
		else
			failedActions.push_back(action);
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	m_Duration += duration.count();

	return failedActions;
}

void Test::PrintInfo() const
{
	std::cout << m_Successes << "/" << m_Attempts << " tests succeeded." << std::endl
		<< 100 * (double)m_Successes / (double)m_Attempts << "% of tests passed." << std::endl
		<< m_Duration << " microseconds taken." << std::endl;
}
