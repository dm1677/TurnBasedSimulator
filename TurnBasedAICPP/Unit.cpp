#include "Unit.h"
#include <iostream>
#include <fstream>
#include <string>

bool Unit::TakeDamage(unsigned char damage)
{
	if (health > damage)
	{
		health -= damage;
		return true;
	}
	return false;
}

void Unit::print() {
	std::cout << "Position: " << (int)x << ", " << (int)y << std::endl;
	std::cout << "Health: " << (int)health << std::endl;
	std::cout << "Unit type: " << (UnitType)unit_type << std::endl;
	std::cout << "Owner: " << (User)owner << std::endl;
}

void Unit::write() {
	std::ofstream file;
	file.open("unit.txt");

	file << "Position: " << (int)x << ", " << (int)y << std::endl;
	file << "Health: " << (int)health << std::endl;
	file << "Unit type: " << unit_type << std::endl;
	file << "Owner: " << owner;

	file.close();
}

void Unit::read() {
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
	switch (unit_type) {
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

bool Unit::IsMoveAttacker(UnitType unit_type)
{
	switch (unit_type) {
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

char Unit::get_char_representation()
{
	switch (unit_type) {
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

Direction Unit::get_direction() const {
	switch (unit_type) {
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

int Unit::get_speed() const {
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

int Unit::get_speed(UnitType unit_type) {
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
	switch (unit_type) {
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

int Unit::GetMaxHealth(UnitType unit_type) {
	switch (unit_type) {
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

int Unit::get_cost() const{
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

int Unit::get_cost(UnitType unit_type) {
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

int Unit::get_range() const
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

int Unit::get_range(UnitType unit_type)
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