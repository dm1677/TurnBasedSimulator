#include <iostream>
#include "GameState.h"
#include "AI.h"
#include "Match.h"
#include <chrono>
#include <ctime>

#include <fstream>

void CountMoves(GameState state)
{
	auto moves = state.GetLegalMoves();
	std::cout << "Total actions: " << "\n\n" << moves.size();

	int attack = 0;
	int move = 0;
	int create = 0;

	for (auto const& possibleMove : moves) {
		if (possibleMove.GetActionType() == Attack) { attack++; }
		if (possibleMove.GetActionType() == Create) { create++; }
		if (possibleMove.GetActionType() == Move) { move++; }
	}

	std::cout << "\n\nAttack actions: " << attack << "\nMove actions: " << move << "\nCreate actions: " << create;
	std::cout << "\nMoney: " << state.GetMoney(state.GetPlayer()) << std::endl;
}

void ExecuteAndRenderAction(Simulator simulator, Action action)
{
	simulator.GenerateNewState(action);
	simulator.GetCurrentState().DrawGrid();
	CountMoves(simulator.GetCurrentState());
}

void writeUnitsToFile(const std::vector<Unit>& units, const std::string& filename) {
	std::ofstream outFile(filename, std::ios::binary);

	if (!outFile.is_open()) {
		// Handle file opening error
		return;
	}

	for (const Unit& unit : units) {
		uint32_t packedData = 0;

		packedData |= (unit.GetX() & 0x0F);
		packedData |= ((unit.GetY() & 0x0F) << 4);
		packedData |= ((unit.getHealth() & 0xFF) << 8);
		packedData |= ((unit.getUnitType() & 0x07) << 16);
		packedData |= ((unit.getOwner() & 0x03) << 19);

		outFile.write(reinterpret_cast<char*>(&packedData), sizeof(packedData));
	}

	outFile.close();
}

std::vector<Unit> readUnitsFromFile(const std::string& filename) {
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


int main()
{
	//Match match;
	//match.UpdateState(Action(Prawn, 1, 1));

	std::cout << "Test\n";

	/*GameState state;
	state.DrawGrid();

	CountMoves(state);
	Action action(Prawn, 7, 7);
	Simulator simulator(state, action);
	auto newState = simulator.GenerateNewState(action);
	newState.print_units();

	std::cout << "\n\n\n\n________________\n\n\n\n\n";


	writeUnitsToFile(simulator.GetCurrentState().GetUnitData(), "units.bin");
	auto u = readUnitsFromFile("units.bin");
	for (auto uu : u)
	{
		uu.print();
		std::cout << std::endl;
		
	}*/

	system("pause>0");
}

/*
void foo(GameState state, Action action)
{
	auto ai = new AI(&state, action);

	auto start = std::chrono::system_clock::now();
	ai->get_move();
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "\n\n\nElapsed time: " << elapsed_seconds.count() << "s\n";

	delete ai;
}*/