#include <iostream>
#include "GameSimulator/GameState.h"
#include "AI/RandomAI.h"
#include "AI/RulesAI.h"
#include "AI/MinimaxAI.h"
#include "AI/MCTSAI.h"
#include "GameSimulator/Match.h"
#include <chrono>
#include <ctime>

#include "GridRenderer.h"

void simulate(int matchCount, int movesPerMatch, bool draw = false, bool printDebugInfo = false)
{
	for (int j = 0; j < matchCount; j++) {
		Match match;

		match.UpdateState(Action(Create, 0, 0, 13, 13, Gobbo));

		for (int i = 0; i < movesPerMatch; i++) {
			auto state = match.GetCurrentGameState();
			MCTSAI AI(state);
			auto action = AI.GetAction();
			match.UpdateState(action);
			if (draw) match.DrawCurrentState();
			if (printDebugInfo)
			{
				match.GetCurrentGameState().PrintData();
				//match.PrintData();
			}
		}
	}
}

void sim2()
{
	Match match;
	int turn = 0;
	match.DrawCurrentState();
	while (!match.GetCurrentGameState().IsGameOver())
	{
		if (turn % 2 == 0)
		{
			MCTSAI AI(match.GetCurrentGameState());
			std::cout << "AI 1 Move:" << std::endl;
			match.UpdateState(AI.GetAction());
		}
		else
		{
			MCTSAI AI(match.GetCurrentGameState());
			match.UpdateState(AI.GetAction());
			std::cout << "AI 2 Move:" << std::endl;
		}
		match.DrawCurrentState();
		turn++;
	}
	match.PrintData();
	match.CreateReplayFile("FirstAIBattle.tbr");
}

void sim3(int matchCount)
{
	for (int j = 0; j < matchCount; j++) {
		Match match;

		while (!match.GetCurrentGameState().IsGameOver()) {
			MCTSAI AI(match.GetCurrentGameState());
			match.UpdateState(AI.GetAction());
		}
		match.CreateReplayFile(std::to_string(j) + ".tbr");
	}
}

void knight_test()
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
}

void fork_test()
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

void fork_test2()
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

void knight_corner_test()
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
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();
}

void replay()
{
	Match match;
	match.PlayReplayFromFile("3.rep", true);
	match.PrintData();
}

int main()
{
	//simulate(1, 10, true, true);
	
	//sim2();

	//fork_test2();

	//sim3(12);

	//replay();
	
	//fork_test();
	
	for (int i = 0; i < 10; i++)
		fork_test2();
	//knight_test();
	//knight_corner_test();
	
	std::cout << "\n\nDone.";
	system("pause>0");
}