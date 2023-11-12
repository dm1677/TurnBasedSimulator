#include <iostream>
#include "GameSimulator/GameState.h"
#include "AI/RandomAI.h"
#include "AI/RulesAI.h"
#include "AI/MinimaxAI.h"
#include "AI/MCTSAI.h"
#include "GameSimulator/Match.h"
#include "TestManager.h"
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>

#include "GridRenderer.h"
#include "WindowRenderer.h"

std::condition_variable cv;
std::mutex mu;
bool moveReady = false;
bool aiThreadNotRunning = true;

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

void printinfo(const GameState& state)
{
	auto& units = state.GetUnitData();
	auto playerOneMoney = units[0].GetHealth();
	auto playerTwoMoney = units[1].GetHealth();

	int playerKingHealthPool = 0;
	int enemyKingHealthPool = 0;
	const auto& kings = state.GetKings();

	for (const auto& king : kings)
	{
		if (king.GetOwner() == Player)
			playerKingHealthPool += king.GetHealth();
		else if (king.GetOwner() != Neutral)
			enemyKingHealthPool += king.GetHealth();
	}

	std::cout << "Player 1 Total King Health: " << playerKingHealthPool << std::endl;
	std::cout << "Player 2 Total King Health: " << enemyKingHealthPool << std::endl;
	std::cout << "Player 1 Money: " << static_cast<unsigned int>(playerOneMoney) << std::endl;
	std::cout << "Player 2 Money: " << static_cast<unsigned int>(playerTwoMoney) << std::endl << std::endl;
}

void getBestMove(const Match& match, Action& bestAction, bool& actionReady) {
	static int i = 0;
	Action action;

	if (i % 2 == 0) {
		//std::cout << "Getting player 1's move..." << std::endl;
		action = MCTSAI(match.GetCurrentGameState()).GetAction();
	}
	else {
		//std::cout << "Getting player 2's move..." << std::endl;
		action = MCTSAI(match.GetCurrentGameState()).GetAction();
	}

	std::lock_guard<std::mutex> lock(mu);
	bestAction = action; // Update the action to be used by the main thread
	actionReady = true; // Signal that the action is ready
	i++;
}

void sim2()
{
	Match match;
	int turn = 0;

	WindowRenderer renderer;

	//match.DrawCurrentState();
	while (!match.GetCurrentGameState().IsGameOver())
	{
		if (turn % 2 == 0)
		{
			MCTSAI AI(match.GetCurrentGameState());
			std::cout << "AI 1 Move:" << std::endl;
			match.UpdateState(AI.GetAction());
		}
		else
		{
			MCTSAI AI(match.GetCurrentGameState());
			match.UpdateState(AI.GetAction());
			std::cout << "AI 2 Move:" << std::endl;
		}
		//match.DrawCurrentState();
		renderer.RenderState(match.GetCurrentGameState());
		renderer.OnFrame();
		turn++;
	}
	match.PrintData();
	match.CreateReplayFile("FirstAIBattle.tbr");
}

void sim3(int matchCount)
{
	for (int j = 0; j < matchCount; j++) {
		Match match;

		while (!match.GetCurrentGameState().IsGameOver()) {
			MCTSAI AI(match.GetCurrentGameState());
			match.UpdateState(AI.GetAction());
		}
		match.CreateReplayFile(std::to_string(j) + ".tbr");
	}
}

void replay()
{
	Match match;
	match.PlayReplayFromFile("3.rep", true);
	match.PrintData();
}

void updateUI(WindowRenderer& renderer, const Match& match) {
	renderer.RenderState(match.GetCurrentGameState());
	renderer.OnFrame();
}

void mtSim() {
	WindowRenderer renderer;
	Match match;
	Action bestAction;
	bool actionReady = false;

	while (true) {
		if (aiThreadNotRunning && !match.GetCurrentGameState().IsGameOver()) {
			std::thread aiThread(&getBestMove, std::ref(match), std::ref(bestAction), std::ref(actionReady));
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
}

int main()
{
	//TestManager test;
	//test.RunTests();
	
	//sim2();

	mtSim();

	std::cout << "\n\nDone.";
	system("pause>0");
}