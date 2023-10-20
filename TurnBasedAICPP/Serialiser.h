#pragma once
#include "Unit.h"
#include "Action.h"
#include <string>
#include <vector>

class Serialiser
{
public:
	void WriteUnitToFile(const Unit& unit, const std::string& filename) const;
	void ReadUnitFromFile(const std::string& filename) const;
	void WriteUnitsToBinaryFile(const std::vector<Unit>& units, const std::string& filename) const;
	std::vector<Unit> ReadUnitsFromBinaryFile(const std::string& filename) const;
	void WriteReplayToFile(const std::vector<Action>& actions, const std::string& filename) const;
private:
	std::string getActionData(const Action& action, unsigned char owner) const;
};