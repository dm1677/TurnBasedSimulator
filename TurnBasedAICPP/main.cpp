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

void test()
{
	std::vector<Unit> units;

	units.emplace_back(15, 15, 0, Resource, Player);
	units.emplace_back(15, 15, 0, Resource, Enemy);
	units.emplace_back(0, 0, 40, King, Player);
	units.emplace_back(0, 1, 40, King, Enemy);
	//units.emplace_back(0, 1, Unit::GetMaxHealth(Gobbo), Gobbo, Enemy);

	GameState state(units, Player);
	Simulator simulator(state, Action());

	MCTSAI AI(state);
	auto action = AI.GetAction();
	simulator.GenerateNewState(action);
	GridRenderer renderer(simulator.GetCurrentState());
	renderer.DrawGrid();
}

int main()
{
	//simulate(1, 10, true, true);
	test();
	
	std::cout << "\n\nDone.";
	system("pause>0");
}