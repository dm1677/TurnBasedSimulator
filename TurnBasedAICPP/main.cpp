#include <iostream>
#include "GameSimulator/GameState.h"
#include "AI/RandomAI.h"
#include "AI/RulesAI.h"
#include "AI/MinimaxAI.h"
#include "GameSimulator/Match.h"
#include <chrono>
#include <ctime>

void simulate(int matchCount, int movesPerMatch, bool draw = false, bool printDebugInfo = false)
{
	for (int j = 0; j < matchCount; j++) {
		Match match;
		for (int i = 0; i < movesPerMatch; i++) {
			auto state = match.GetCurrentGameState();
			RandomAI AI(state);
			auto action = AI.GetAction();
			match.UpdateState(action);
			if (draw) match.DrawCurrentState();
		}
		if (printDebugInfo)
		{
			match.GetCurrentGameState().PrintData();
			match.PrintData();
		}
	}
}
#include "BitBoard.h"
int main()
{
	BitBoard board;
	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			if (board.IsSet(x, y)) std::cout << "error at " << x << ", " << y << std::endl;
		}
	}
	system("pause>0");
}