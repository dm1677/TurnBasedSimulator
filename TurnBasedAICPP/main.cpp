#include <iostream>
#include "GameState.h"
#include "RandomAI.h"
#include "Match.h"
#include <chrono>
#include <ctime>

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
	//simulator.GetCurrentState().DrawGrid();
	CountMoves(simulator.GetCurrentState());
}


int main()
{
	Match match;

	for (int i = 0; i < 10; i++) {
		auto state = match.GetCurrentGameState();
		RandomAI randomAI(state);
		auto action = randomAI.GetAction();
		match.UpdateState(action);
	}


	//auto action = randomAI.GetAction();
	//auto action = Action(Prawn, 2, 2);
	//match.UpdateState(action);

	/*
	match.UpdateState(Action(Prawn, 7, 7));
	match.UpdateState(Action(Prawn, 1, 1));
	match.UpdateState(Action(Gobbo, 3, 3));
	match.UpdateState(Action(Gobbo, 4, 4));

	match.CreateReplayFile();*/

	//match.PlayReplayFromFile("replay.tbr");

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