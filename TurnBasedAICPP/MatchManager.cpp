#include "MatchManager.h"

namespace TurnBasedSimulator
{
	void MatchManager::Simulate(int matchCount, int movesPerMatch, bool draw, bool printDebugInfo) const
	{
		for (int j = 0; j < matchCount; j++) {
			Match match;

			for (int i = 0; i < movesPerMatch; i++) {
				auto state = match.GetCurrentGameState();
				RandomAI AI(state);
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

	User MatchManager::RenderAndSim() {
		WindowRenderer renderer;
		Match match;
		Action bestAction;
		bool actionReady = false;

		while (!match.IsGameOver())
		{
			if (aiThreadNotRunning && !match.GetCurrentGameState().IsGameOver())
			{
				std::thread aiThread(&MatchManager::getBestMove, this, std::ref(match), std::ref(bestAction), std::ref(actionReady));
				aiThread.detach();
				aiThreadNotRunning = false;
			}

			// Check if the AI has completed its move
			{
				std::lock_guard<std::mutex> lock(mu);
				if (actionReady) {
					match.UpdateState(bestAction); // Update the game state with the AI's move
					//printinfo(match.GetCurrentGameState()); // Now safe to print the game state
					updateUI(renderer, match); // Update the UI with the new move
					actionReady = false;
					aiThreadNotRunning = true;
				}
			}

			// Render the current state
			updateUI(renderer, match);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		std::cout << "End of game." << std::endl;
		//match.CreateReplayFile("output\\test\\replay.tbr");

		return match.GetWinner();
	}

	void MatchManager::SimAndWins(int sims)
	{
		int wins[] = { 0, 0 };

		std::cout << "Simulating " << sims << " games..." << std::endl;

		for (int i = 0; i < sims; i++)
			wins[(int)RenderAndSim()]++;

		std::cout << "Finished simulating." << std::endl;
		std::cout << "Player 1 wins: " << wins[0] << " = " << ((double)wins[0] / (double)sims) * 100.0 << "% winrate. " << std::endl;
		std::cout << "Player 2 wins: " << wins[1] << " = " << ((double)wins[1] / (double)sims) * 100.0 << "% winrate. " << std::endl;
	}
}