#pragma once
#include "../GameSimulator/Simulator.h"
#include "../GridRenderer.h"
#include "../AI/MCTSAI.h"

class TestManager
{
public:

	void RunTests() const;

private:

	bool knight_OHK() const;
	bool knight_escape() const;
	bool gobbo_OHK() const;
	bool king_fork() const;
	bool weighted_king_fork() const;
	bool knight_corner() const;

};

