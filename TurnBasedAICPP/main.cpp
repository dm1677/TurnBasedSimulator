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
	int matchCount = 1;
	int movesPerMatch = 400;

	auto start = std::chrono::system_clock::now();
	for (int j = 0; j < matchCount; j++) {
		Match match;
		for (int i = 0; i < movesPerMatch; i++) {
			auto state = match.GetCurrentGameState();
			RandomAI randomAI(state);
			auto action = randomAI.GetAction();
			match.UpdateState(action);
			match.DrawCurrentState();
		}
	}
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "\n\n\nElapsed time: " << elapsed_seconds.count() << "s\n";

	system("pause>0");
}