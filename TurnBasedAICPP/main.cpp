#include <iostream>
#include "GameSimulator/GameState.h"
#include "AI/RandomAI.h"
#include "GameSimulator/Match.h"
#include <chrono>
#include <ctime>

void simulate(int matchCount, int movesPerMatch, bool draw = false)
{
	for (int j = 0; j < matchCount; j++) {
		Match match;
		for (int i = 0; i < movesPerMatch; i++) {
			auto state = match.GetCurrentGameState();
			RandomAI randomAI(state);
			auto action = randomAI.GetAction();
			match.UpdateState(action);
			if (draw) match.DrawCurrentState();
		}
		//match.CreateReplayFile("output/rep.tbr");
	}
}

int main()
{
	simulate(1, 100, true);

	system("pause>0");
}