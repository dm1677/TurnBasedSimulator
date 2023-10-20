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