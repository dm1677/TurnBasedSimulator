#include "Unit.h"
#include <iostream>
#include <fstream>
#include <string>

bool Unit::TakeDamage(unsigned char damage)
{
	if (m_Health > damage)
	{
		m_Health -= damage;
		return true;
	}
	return false;
}

void Unit::Print() {
	std::cout << "Position: " << (int)m_X << ", " << (int)m_Y << std::endl;
	std::cout << "Health: " << (int)m_Health << std::endl;
	std::cout << "Unit type: " << (UnitType)m_UnitType << std::endl;
	std::cout << "Owner: " << (User)m_Owner << std::endl;
}

void Unit::Write() {
	std::ofstream file;
	file.open("unit.txt");

	file << "Position: " << (int)m_X << ", " << (int)m_Y << std::endl;
	file << "Health: " << (int)m_Health << std::endl;
	file << "Unit type: " << m_UnitType << std::endl;
	file << "Owner: " << m_Owner;

	file.close();
}

void Unit::Read() {
	std::ifstream file("unit.txt");
	std::string text;

	while (getline(file, text))
	{
		std::cout << text;
	}

	file.close();
}

bool Unit::IsMoveAttacker() const
{
	switch (m_UnitType) {
	case (int)Prawn:
		return true;
	case (int)Building:
		return false;
	case (int)King:
		return false;
	case (int)Knight:
		return true;
	case (int)Gobbo:
		return true;
	default:
		return Line;
	}
}

bool Unit::IsMoveAttacker(UnitType unitType)
{
	switch (unitType) {
	case (int)Prawn:
		return true;
	case (int)Building:
		return false;
	case (int)King:
		return false;
	case (int)Knight:
		return true;
	case (int)Gobbo:
		return true;
	default:
		return Line;
	}
}

char Unit::GetCharRepresentation() const
{
	switch (m_UnitType) {
	case (int)Prawn:
		return 'P';
	case (int)Building:
		return '+';
	case (int)King:
		return 'K';
	case (int)Knight:
		return 'T';
	case (int)Gobbo:
		return 'G';
	case (int)Tree:
		return 'Y';
	case (int)Resource:
		return '$';
	default:
		return 0;
	}
}

Direction Unit::GetDirection() const {
	switch (m_UnitType) {
	case (int)Prawn:
		return Line;
	case (int)Building:
		return Line;
	case (int)King:
		return Line;
	case (int)Knight:
		return LineAndDiagonal;
	case (int)Gobbo:
		return Diagonal;
	default:
		return Line;
	}
}

int Unit::GetSpeed() const {
	switch (m_UnitType) {
	case (int)Prawn:
		return 2;
	case (int)Building:
		return 0;
	case (int)King:
		return 2;
	case (int)Knight:
		return 1;
	case (int)Gobbo:
		return 3;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

int Unit::GetSpeed(UnitType unit_type) {
	switch (unit_type) {
	case (int)Prawn:
		return 2;
	case (int)Building:
		return 0;
	case (int)King:
		return 2;
	case (int)Knight:
		return 1;
	case (int)Gobbo:
		return 3;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

int Unit::GetMaxHealth() const{
	switch (m_UnitType) {
	case (int)Prawn:
		return 13;
	case (int)Building:
		return 20;
	case (int)King:
		return 40;
	case (int)Knight:
		return 24;
	case (int)Gobbo:
		return 8;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

int Unit::GetMaxHealth(UnitType unitType) {
	switch (unitType) {
	case (int)Prawn:
		return 13;
	case (int)Building:
		return 20;
	case (int)King:
		return 40;
	case (int)Knight:
		return 24;
	case (int)Gobbo:
		return 8;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

unsigned char Unit::GetDamage() const{
	switch (m_UnitType) {
	case (int)Prawn:
		return 5;
	case (int)Building:
		return 8;
	case (int)King:
		return 8;
	case (int)Knight:
		return 12;
	case (int)Gobbo:
		return 5;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

unsigned char Unit::GetDamage(UnitType unit_type) {
	switch (unit_type) {
	case (int)Prawn:
		return 5;
	case (int)Building:
		return 8;
	case (int)King:
		return 8;
	case (int)Knight:
		return 12;
	case (int)Gobbo:
		return 5;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

unsigned char Unit::GetCost() const{
	switch (m_UnitType){
	case (int)Prawn:
		return 5;
	case (int)Building:
		return 8;
	case (int)King:
		return 0;
	case (int)Knight:
		return 12;
	case (int)Gobbo:
		return 3;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

unsigned char Unit::GetCost(UnitType unit_type) {
	switch (unit_type) {
	case (int)Prawn:
		return 5;
	case (int)Building:
		return 8;
	case (int)King:
		return 0;
	case (int)Knight:
		return 12;
	case (int)Gobbo:
		return 3;
	case (int)Tree:
		return 0;
	case (int)Resource:
		return 0;
	default:
		return 0;
	}
}

int Unit::GetRange() const
{
	switch (m_UnitType) {
	case (int)Prawn:
		return 4;
	case (int)Building:
		return 5;
	case (int)King:
		return 3;
	case (int)Knight:
		return 1;
	case (int)Gobbo:
		return 1;
	default:
		return 0;
	}
}

int Unit::GetRange(UnitType unit_type)
{
	switch (unit_type) {
	case (int)Prawn:
		return 4;
	case (int)Building:
		return 5;
	case (int)King:
		return 3;
	case (int)Knight:
		return 1;
	case (int)Gobbo:
		return 1;
	default:
		return 0;
	}
}