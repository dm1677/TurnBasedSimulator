#include <iostream>
#include "GameState.h"
#include "AI.h"
#include "Match.h"
#include <chrono>
#include <ctime>

#include <fstream>

void CountMoves(GameState state)
{
	auto moves = state.GetLegalMoves();
	std::cout << "Total actions: " << "\n\n" << moves.size();

	int attack = 0;
	int move = 0;
	int create = 0;

	for (auto const& possibleMove : moves) {
		if (possibleMove.GetActionType() == Attack) { attack++; }
		if (possibleMove.GetActionType() == Create) { create++; }
		if (possibleMove.GetActionType() == Move) { move++; }
	}

	std::cout << "\n\nAttack actions: " << attack << "\nMove actions: " << move << "\nCreate actions: " << create;
	std::cout << "\nMoney: " << state.GetMoney(state.GetPlayer()) << std::endl;
}

void ExecuteAndRenderAction(Simulator simulator, const Action& action)
{
	simulator.GenerateNewState(action);
	simulator.GetCurrentState().DrawGrid();
	CountMoves(simulator.GetCurrentState());
}


int main()
{
	Match match;
	match.UpdateState(Action(Prawn, 7, 7));


	/*GameState state;
	state.DrawGrid();

	CountMoves(state);
	Action action(Prawn, 7, 7);
	Simulator simulator(state, action);
	auto newState = simulator.GenerateNewState(action);
	newState.print_units();

	std::cout << "\n\n\n\n________________\n\n\n\n\n";


	writeUnitsToFile(simulator.GetCurrentState().GetUnitData(), "units.bin");
	auto u = readUnitsFromFile("units.bin");
	for (auto uu : u)
	{
		uu.print();
		std::cout << std::endl;
		
	}*/

	system("pause>0");
}

/*
void foo(GameState state, Action action)
{
	auto ai = new AI(&state, action);

	auto start = std::chrono::system_clock::now();
	ai->get_move();
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "\n\n\nElapsed time: " << elapsed_seconds.count() << "s\n";

	delete ai;
}*/