#include "TestManager.h"
#include "Test.h"

typedef bool (TestManager::* TestFunction)() const;

//void TestManager::RunTests() const
//{
//	const int attempts = 10;
//	int successes = 0;
//
//	std::vector<TestFunction> tests;
//	tests.push_back(&TestManager::knight_escape);
//	tests.push_back(&TestManager::knight_OHK);
//	tests.push_back(&TestManager::knight_corner);
//	tests.push_back(&TestManager::king_fork);
//	tests.push_back(&TestManager::weighted_king_fork);
//	tests.push_back(&TestManager::gobbo_OHK);
//
//	for (const auto& test : tests)
//	{
//		for (int i = 0; i < attempts; i++)
//		{
//			if ((this->*test)())
//				successes++;
//			else
//				std::cout << "Test failed." << std::endl;
//		}
//	}
//
//	std::cout << successes << "/" << attempts << " tests succeeded." << std::endl << 100 * (double)successes/(double)attempts << "% of tests passed." << std::endl;
//}

void TestManager::RunTests() const
{
	std::vector<Unit> units;
	units.emplace_back(0, 0, 10, King, Player);
	units.emplace_back(0, 14, 40, King, Enemy);
	units.emplace_back(0, 1, Unit::GetMaxHealth(Knight), Knight, Enemy);

	std::vector<Action> correctActions;
	correctActions.emplace_back(2, 2, 0);

	Puzzle puzzle(0, 0, units, correctActions);

	Test test(puzzle);

	const AI* ai = new MCTSAI(test.GetState(), true, false);
	auto wrongMoves = test.Run(ai, 10);
	std::cout << wrongMoves.size() << std::endl;
	delete ai;

	GridRenderer renderer(test.GetState());
	renderer.DrawGrid();
}

bool TestManager::knight_OHK() const
{
	std::vector<Unit> units;

	units.emplace_back(15, 15, 0, Resource, Player);
	units.emplace_back(15, 15, 0, Resource, Enemy);
	units.emplace_back(0, 0, 10, King, Player);
	units.emplace_back(0, 14, 40, King, Enemy);
	units.emplace_back(0, 1, Unit::GetMaxHealth(Knight), Knight, Enemy);

	GameState state(units, Player);
	Simulator simulator(state, Action());

	MCTSAI AI(state, false, false);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);

	auto result = action == Action(2, 2, 0);
	if (!result)
	{
		GridRenderer renderer(simulator.GetCurrentState());
		renderer.DrawGrid();
	}

	return result;
}

bool TestManager::knight_escape() const
{
	std::vector<Unit> units;

	units.emplace_back(15, 15, 0, Resource, Player);
	units.emplace_back(15, 15, 0, Resource, Enemy);
	units.emplace_back(0, 0, 40, King, Player);
	units.emplace_back(0, 14, 40, King, Enemy);
	units.emplace_back(0, 1, Unit::GetMaxHealth(Knight), Knight, Enemy);

	GameState state(units, Player);
	Simulator simulator(state, Action());

	MCTSAI AI(state, false, false);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();

	return action == Action(2, 2, 0);
}

bool TestManager::gobbo_OHK() const
{
	std::vector<Unit> units;

	units.emplace_back(15, 15, 0, Resource, Player);
	units.emplace_back(15, 15, 0, Resource, Enemy);
	units.emplace_back(0, 0, 5, King, Player);
	units.emplace_back(0, 14, 40, King, Enemy);
	units.emplace_back(1, 1, Unit::GetMaxHealth(Gobbo), Gobbo, Enemy);

	GameState state(units, Player);
	Simulator simulator(state, Action());

	MCTSAI AI(state, false, false);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();

	auto correctActions = std::array<Action, 2>();
	correctActions[0] = Action(2, 1, 0);
	correctActions[1] = Action(2, 0, 1);

	for (auto& a : correctActions)
		if (action == a) return true;
	return false;
}

bool TestManager::king_fork() const
{
	std::vector<Unit> units;

	units.emplace_back(15, 15, 8, Resource, Player);
	units.emplace_back(15, 15, 0, Resource, Enemy);
	units.emplace_back(0, 0, 40, King, Player);
	units.emplace_back(14, 0, 40, King, Player);
	units.emplace_back(0, 14, 40, King, Enemy);
	units.emplace_back(8, 14, 40, King, Enemy);

	GameState state(units, Player);
	Simulator simulator(state, Action());

	MCTSAI AI(state, false, false);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();

	auto correctActions = std::array<Action, 7>();
	for (int i = 2; i < 5; i++)
		correctActions[i] = Action(Building, i + 1, 14);

	for (auto& a : correctActions)
		if (action == a) return true;
	return false;
}

bool TestManager::weighted_king_fork() const
{
	std::vector<Unit> units;

	units.emplace_back(15, 15, 8, Resource, Player);
	units.emplace_back(15, 15, 0, Resource, Enemy);
	units.emplace_back(0, 0, 40, King, Player);
	units.emplace_back(0, 14, 40, King, Enemy);
	units.emplace_back(8, 14, 40, King, Enemy);

	GameState state(units, Player);
	Simulator simulator(state, Action());

	MCTSAI AI(state, false, false);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();

	auto correctActions = std::array<Action, 7>();
	for (int i = 2; i < 5; i++)
		correctActions[i] = Action(Building, i + 1, 14);

	for (auto& a : correctActions)
		if (action == a) return true;
	return false;
}

bool TestManager::knight_corner() const
{
	std::vector<Unit> units;

	units.emplace_back(15, 15, 12, Resource, Player);
	units.emplace_back(15, 15, 0, Resource, Enemy);
	units.emplace_back(0, 0, 40, King, Player);
	units.emplace_back(0, 14, 40, King, Enemy);

	GameState state(units, Player);
	Simulator simulator(state, Action());

	MCTSAI AI(state, false, false);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);

	auto result = action == Action(Knight, 1, 13);
	if (!result)
	{ 
		GridRenderer renderer(simulator.GetCurrentState());
		renderer.DrawGrid();
	}

	return result;
}