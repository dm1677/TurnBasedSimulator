#include "Serialiser.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void Serialiser::WritePuzzleToFile(const Puzzle& puzzle, const std::string& filename) const
{
	std::ofstream outFile(filename, std::ios::binary);

	const auto& name = puzzle.GetName();
	const auto& units = puzzle.GetState().GetUnitData();
	const auto& correctActions = puzzle.GetCorrectActions();

	size_t nameLength = name.length();
	size_t unitsLength = units.size();
	size_t actionsLength = correctActions.size();

	outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
	outFile.write(reinterpret_cast<char*>(&unitsLength), sizeof(unitsLength));
	outFile.write(reinterpret_cast<char*>(&actionsLength), sizeof(actionsLength));

	outFile.write(name.c_str(), nameLength);

	for (const auto& unit : units)
	{
		auto data = unit.ToBinary();
		outFile.write(reinterpret_cast<char*>(&data), sizeof(data));
	}

	for (const auto& action : correctActions)
	{
		auto data = action.ToBinary();
		outFile.write(reinterpret_cast<char*>(&data), sizeof(data));
	}

	outFile.close();
}

Puzzle Serialiser::ReadPuzzleFromFile(const std::string& filename) const
{
	//return Puzzle(1, 1, std::vector<Unit>(), std::vector<Action>());
	std::ifstream inFile(filename, std::ios::binary);

	if (!inFile.is_open()) {
		// Handle file opening error
		std::cout << "Error opening puzzle file.";
	}

	size_t nameLength;
	size_t unitsLength;
	size_t actionsLength;

	inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
	inFile.read(reinterpret_cast<char*>(&unitsLength), sizeof(unitsLength));
	inFile.read(reinterpret_cast<char*>(&actionsLength), sizeof(actionsLength));

	std::string name;
	std::vector<Unit> units;
	std::vector<Action> correctActions;

	name.resize(nameLength);
	units.reserve(unitsLength);
	correctActions.reserve(actionsLength);

	inFile.read(&name[0], nameLength);
	
	for (int i = 0; i < unitsLength; i++)
	{
		uint32_t packedData;
		inFile.read(reinterpret_cast<char*>(&packedData), sizeof(packedData));
		units.emplace_back(packedData);
	}

	for (int i = 0; i < actionsLength; i++)
	{
		uint32_t packedData;
		inFile.read(reinterpret_cast<char*>(&packedData), sizeof(packedData));
		correctActions.emplace_back(packedData);
	}

	if (inFile.peek() != EOF)
	{
		std::cout << "HMM";
	}

	inFile.close();
	return Puzzle(GameState(units, Player), correctActions, name);
}

std::vector<Puzzle> Serialiser::GetPuzzlesFromDirectory(const std::string& directoryPath) const
{
	std::vector<Puzzle> puzzles;

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".tbp") {
			Puzzle puzzle = ReadPuzzleFromFile(entry.path().string());
			puzzles.push_back(puzzle);
		}
	}
	return puzzles;
}

void Serialiser::WriteUnitsToBinaryFile(const std::vector<Unit>& units, const std::string& filename) const {
	std::ofstream outFile(filename, std::ios::binary);

	if (!outFile.is_open()) {
		// Handle file opening error
		return;
	}

	for (const Unit& unit : units) {
		auto packedData = unit.ToBinary();
		outFile.write(reinterpret_cast<char*>(&packedData), sizeof(packedData));
	}

	outFile.close();
}

std::vector<Unit> Serialiser::ReadUnitsFromBinaryFile(const std::string& filename) const {
	std::vector<Unit> units;
	std::ifstream inFile(filename, std::ios::binary);

	if (!inFile.is_open()) {
		// Handle file opening error
		return units;
	}

	while (inFile.peek() != EOF) {
		uint32_t packedData;
		inFile.read(reinterpret_cast<char*>(&packedData), sizeof(packedData));

		units.emplace_back(packedData);
	}

	inFile.close();
	return units;
}

void Serialiser::WriteReplayToFile(const std::vector<Action>& actions, const std::string& filename) const
{
	std::ofstream file(filename);

	unsigned char owner = 1;
	for (const Action& action : actions)
	{
		owner = (owner == 0) ? 1 : 0;
		file << ";" << std::endl;
		file << getActionData(action, owner);
	}
	file << ";";
	file.close();
}

std::vector<Action> Serialiser::ReadReplayFile(const std::string& filename) const
{
	std::ifstream file(filename);
	std::string line;
	std::vector<Action> actions;
	std::vector<std::string> actionData;
	
	while (getline(file, line))
	{
		if (line == ";")
		{
			if (!actionData.empty())
			{
				actions.push_back(deserialiseAction(actionData));
				actionData.clear();
			}
		}
		else actionData.push_back(line);
	}
	return actions;
}

Action Serialiser::deserialiseAction(const std::vector<std::string>& actionData) const
{
	if (actionData.empty()) return Action();
	const std::string& actionType = actionData[0];
	if (actionType == "CreateAction")
	{
		if (actionData.size() != 6) return Action();
		return Action(static_cast<UnitType>(std::stoi(actionData[3])),
			          static_cast<unsigned char>(std::stoi(actionData[1])),
			          static_cast<unsigned char>(std::stoi(actionData[2])));
	}
	if (actionType == "MoveAction")
	{
		if (actionData.size() != 4) return Action();
		return Action(static_cast<unsigned char>(std::stoi(actionData[1])),
			          static_cast<unsigned char>(std::stoi(actionData[2])),
			          static_cast<unsigned char>(std::stoi(actionData[3])));
	}
	if (actionType == "AttackAction")
	{
		if (actionData.size() != 3) return Action();
		return Action(std::stoi(actionData[1]),
			          std::stoi(actionData[2]));
	}
	if (actionType == "SwapAction")
	{
		if (actionData.size() != 3) return Action();
		return Action(Swap,
					  std::stoi(actionData[1]),
					  std::stoi(actionData[2]));
	}
	return Action();
}

std::string Serialiser::getActionData(const Action& action, unsigned char owner) const
{
	std::string string = "";
	switch (action.GetActionType())
	{
	case Create:
		string += "CreateAction\n";
		string += std::to_string(action.GetX());
		string += "\n";
		string += std::to_string(action.GetY());
		string += "\n";
		string += std::to_string(action.GetUnitType());
		string += "\n";
		string += std::to_string(owner);
		string += "\n";
		string += std::to_string(owner + 9);
		string += "\n";
		break;
	case Move:
		string += "MoveAction\n";
		string += std::to_string(action.GetUnit1());
		string += "\n";
		string += std::to_string(action.GetX());
		string += "\n";
		string += std::to_string(action.GetY());
		string += "\n";
		break;
	case Attack:
		string += "AttackAction\n";
		string += std::to_string(action.GetUnit1());
		string += "\n";
		string += std::to_string(action.GetUnit2());
		string += "\n";
		break;
	case Swap:
		string += "SwapAction\n";
		string += std::to_string(action.GetUnit1());
		string += "\n";
		string += std::to_string(action.GetUnit2());
		string += "\n";
	}
	return string;
}