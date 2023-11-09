#include "TestManager.h"

void TestManager::RunTests() const
{
	const int attempts = 15;
	int successes = 0;
	for (int i = 0; i < attempts; i++)
	{
		if (knight_corner())
			successes++;
		else
			std::cout << "Test failed." << std::endl;
	}

	std::cout << successes << "/" << attempts << " tests succeeded." << std::endl << 10 * (double)successes/(double)attempts << "% of tests passed." << std::endl;
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

	MCTSAI AI(state);
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

	MCTSAI AI(state);
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

	MCTSAI AI(state);
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

	MCTSAI AI(state);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();
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

	MCTSAI AI(state);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();
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

	MCTSAI AI(state);
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