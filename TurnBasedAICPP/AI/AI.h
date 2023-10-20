#pragma once
#include "../GameSimulator/Action.h"

class AI
{
public:
	virtual Action GetAction() const = 0;
};

