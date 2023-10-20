#pragma once
#include "Action.h"
#include "GameState.h"

class AI
{
public:
	virtual Action GetAction() const = 0;
};

