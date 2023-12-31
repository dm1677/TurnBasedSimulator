#pragma once
#include "GameSimulator/Unit.h"
#include "GameSimulator/Action.h"
#include "GameSimulator/GameState.h"
#include "Testing/Test.h"
#include <string>
#include <vector>

namespace TurnBasedSimulator
{
	class Serialiser
	{
	public:
		void WritePuzzleToFile(const Puzzle& puzzle, const std::string& filename) const;
		Puzzle ReadPuzzleFromFile(const std::string& filename) const;
		std::vector<Puzzle> GetPuzzlesFromDirectory(const std::string& directoryPath) const;
		void WriteUnitsToBinaryFile(const std::vector<Unit>& units, const std::string& filename) const;
		void WriteStateToFile(const GameState& state, const std::string& filename) const { WriteUnitsToBinaryFile(state.GetUnitData(), filename); }
		std::vector<Unit> ReadUnitsFromBinaryFile(const std::string& filename) const;
		GameState ReadStateFromFile(const std::string& filename) const { auto units = ReadUnitsFromBinaryFile(filename); return GameState(units, Player); } //TODO: allow encoding of player to move
		void WriteReplayToFile(const std::vector<Action>& actions, const std::string& filename) const;
		std::vector<Action> ReadReplayFile(const std::string& filename) const;
	private:
		std::string getActionData(const Action& action, unsigned char owner) const;
		Action deserialiseAction(const std::vector<std::string>& actionData) const;
	};
}