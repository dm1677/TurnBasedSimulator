#pragma once
#include "../GameSimulator/Action.h"

namespace TurnBasedSimulator
{
	class AI
	{
	public:
		virtual Action GetAction() const = 0;
	};
}
