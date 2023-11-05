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
			MinimaxAI AI(state);
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

int main()
{
	simulate(1, 10, true, true);
	
	system("pause>0");
}