#include <iostream>

#include "Testing/TestManager.h"
#include "MatchManager.h"

int main()
{
	TurnBasedSimulator::MatchManager matchManager;
	matchManager.RenderAndSim();

	std::cout << "\n\nDone.";
	system("pause>0");
}