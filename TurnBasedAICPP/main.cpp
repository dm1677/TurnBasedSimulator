#include <iostream>
#include "GameSimulator/GameState.h"
#include "AI/RandomAI.h"
#include "GameSimulator/Match.h"
#include <chrono>
#include <ctime>

int main()
{
	Match match;
	match.UpdateState(Action(Prawn, 3, 3));
	match.DrawCurrentState();
	match.UpdateState(Action(Gobbo, 9, 9));
	match.DrawCurrentState();
	match.UpdateState(Action(Swap, 14, 2, 0, 0, Prawn));
	match.DrawCurrentState();
	/*
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
	*/
	system("pause>0");
}