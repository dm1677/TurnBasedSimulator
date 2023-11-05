#include <iostream>
#include "GameSimulator/GameState.h"
#include "AI/RandomAI.h"
#include "AI/RulesAI.h"
#include "AI/MinimaxAI.h"
#include "AI/MCTSAI.h"
#include "GameSimulator/Match.h"
#include <chrono>
#include <ctime>

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

int main()
{
	simulate(1, 10, true, true);
	
	std::cout << "\n\nDone.";
	system("pause>0");
}