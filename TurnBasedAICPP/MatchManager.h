#pragma once
#include <numeric>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>

#include "GameSimulator/Match.h"
#include "AI/RandomAI.h"
#include "AI/MCTSAI.h"
#include "Rendering/WindowRenderer.h"

namespace TurnBasedSimulator
{
	class MatchManager
	{
	public:
		void Simulate(int matchCount, int movesPerMatch, bool draw = false, bool printDebugInfo = false) const;
		User RenderAndSim();
		void SimAndWins(int sims);
	private:
		std::condition_variable cv;
		std::mutex mu;
		bool moveReady = false;
		bool aiThreadNotRunning = true;



		template <typename T>
		double getAverage(const std::vector<T>& vec) {
			if (vec.empty()) {
				return 0.0; // Handle the case of an empty vector
			}
			// Calculate the sum of the vector's elements
			T sum = std::accumulate(vec.begin(), vec.end(), static_cast<T>(0));
			// Calculate the average by dividing sum by the number of elements
			return static_cast<double>(sum) / vec.size();
		}

		void simulateFull(int matchCount)
		{
			std::vector<long long> moveTimes;
			moveTimes.reserve(matchCount * 1000);
			for (int j = 0; j < matchCount; j++) {
				Match match;

				while (!match.IsGameOver())
				{
					auto start = std::chrono::high_resolution_clock::now();
					RandomAI AI(match.GetCurrentGameState(), true);
					match.UpdateState(AI.GetAction());
					auto stop = std::chrono::high_resolution_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
					moveTimes.push_back(duration.count());
				}
			}
			auto avg = getAverage(moveTimes);
			std::cout << "Average time per move: " << avg << "ms" << std::endl;
		}

		Action getPlayerMove(const GameState& state) {
			std::string inputLine;
			while (true)
			{
				std::cout << "Enter move: ";
				std::getline(std::cin, inputLine);
				std::istringstream iss(inputLine);
				std::string command;
				iss >> command;

				if (command == "get")
				{
					int x, y;
					if (iss >> x >> y && x >= 0 && x <= 14 && y >= 0 && y <= 14)
					{
						const std::vector<Unit>& units = state.GetUnitData();
						for (int i = 2; i < units.size(); i++)
						{
							if (units[i].GetX() == x && units[i].GetY() == y)
							{
								std::cout << "Unit ID: " << i << std::endl;
								break;
							}
						}
					}
				}
				else if (command == "action")
				{
					std::string actionTypeStr, unitTypeStr;
					int unit1, unit2, x, y;
					if (iss >> actionTypeStr >> unit1 >> unit2 >> x >> y >> unitTypeStr) {
						ActionType actionType;
						UnitType unitType;

						// Convert actionTypeStr to ActionType
						if (actionTypeStr == "Move") actionType = Move;
						else if (actionTypeStr == "Attack") actionType = Attack;
						else if (actionTypeStr == "Create") actionType = Create;
						else if (actionTypeStr == "Swap") actionType = Swap;

						// Convert unitTypeStr to UnitType
						if (unitTypeStr == "Prawn") unitType = Prawn;
						else if (unitTypeStr == "Building") unitType = Building;
						else if (unitTypeStr == "King") unitType = King;
						else if (unitTypeStr == "Knight") unitType = Knight;
						else if (unitTypeStr == "Gobbo") unitType = Gobbo;
						else if (unitTypeStr == "Tree") unitType = Tree;

						return Action(actionType, unit1, unit2, x, y, unitType);
					}
				}
			}
		}

		void getBestMove(const Match& match, Action& bestAction, bool& actionReady) {
			static int i = 0;
			Action action;

			if (i % 2 == 0) {
				//std::cout << "Getting player 1's move..." << std::endl;
				action = MCTSAI(match.GetCurrentGameState(), false, false).GetAction();
			}
			else {
				//std::cout << "Getting player 2's move..." << std::endl;
				//action = MCTSAI(match.GetCurrentGameState(), false, false).GetAction();
				action = RandomAI(match.GetCurrentGameState(), true).GetAction();
			}

			std::lock_guard<std::mutex> lock(mu);
			bestAction = action; // Update the action to be used by the main thread
			actionReady = true; // Signal that the action is ready
			i++;
		}

		void updateUI(WindowRenderer& renderer, const Match& match) {
			renderer.RenderState(match.GetCurrentGameState());
			renderer.OnFrame();
		}

		

		void playReplay(const std::string& filename)
		{
			WindowRenderer renderer;
			Match match;
			match.LoadReplayFromFile(filename);

			while (true)
			{
				if (!match.GetCurrentGameState().IsGameOver() && renderer.IsRightPressed())
					match.AdvanceReplay();
				updateUI(renderer, match);
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}
	};
}