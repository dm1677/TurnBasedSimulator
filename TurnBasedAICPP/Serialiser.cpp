#include "Serialiser.h"
#include <iostream>
#include <fstream>

void Serialiser::WriteUnitToFile(const Unit& unit, const std::string& filename) const
{
	std::ofstream file;
	file.open(filename);

	file << "Position: " << (int)unit.GetX() << ", " << (int)unit.GetY() << std::endl;
	file << "Health: " << (int)unit.GetHealth() << std::endl;
	file << "Unit type: " << unit.GetUnitType() << std::endl;
	file << "Owner: " << unit.GetOwner();

	file.close();
}

void Serialiser::ReadUnitFromFile(const std::string& filename) const
{
	std::ifstream file(filename);
	std::string text;

	while (getline(file, text))
	{
		std::cout << text;
	}

	file.close();
}

void Serialiser::WriteUnitsToBinaryFile(const std::vector<Unit>& units, const std::string& filename) const {
	std::ofstream outFile(filename, std::ios::binary);

	if (!outFile.is_open()) {
		// Handle file opening error
		return;
	}

	for (const Unit& unit : units) {
		uint32_t packedData = 0;

		packedData |= (unit.GetX() & 0x0F);
		packedData |= ((unit.GetY() & 0x0F) << 4);
		packedData |= ((unit.GetHealth() & 0xFF) << 8);
		packedData |= ((unit.GetUnitType() & 0x07) << 16);
		packedData |= ((unit.GetOwner() & 0x03) << 19);

		outFile.write(reinterpret_cast<char*>(&packedData), sizeof(packedData));
	}

	outFile.close();
}

std::vector<Unit> Serialiser::ReadUnitsFromBinaryFile(const std::string& filename) const {
	std::vector<Unit> units;
	std::ifstream inFile(filename, std::ios::binary);

	if (!inFile.is_open()) {
		// Handle file opening error
		return units; // Return an empty vector
	}

	while (inFile.peek() != EOF) {
		uint32_t packedData;
		inFile.read(reinterpret_cast<char*>(&packedData), sizeof(packedData));

		Unit unit(
			packedData & 0x0F,
			(packedData >> 4) & 0x0F,
			(packedData >> 8) & 0xFF,
			(packedData >> 16) & 0x07,
			(packedData >> 19) & 0x03
		);

		units.push_back(unit);
	}

	inFile.close();
	return units;
}

void Serialiser::WriteReplayToFile(const std::vector<Action>& actions, const std::string& filename) const
{
	std::ofstream file;
	file.open(filename);

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

std::vector<Action>* Serialiser::ReadReplayFile(const std::string& filename) const
{
	std::ifstream file;
	std::string line;
	std::vector<Action>* actions = new std::vector<Action>();
	std::vector<std::string> actionData;
	file.open(filename);
	
	while (getline(file, line))
	{
		if (line == ";")
		{
			if (!actionData.empty())
			{
				Action action = deserialiseAction(actionData);
				actions->push_back(action);
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
		string += "\n";
		string += std::to_string(action.GetUnit1());
		string += "\n";
		string += std::to_string(action.GetUnit2());
		string += "\n";
		break;
	}
	return string;
}