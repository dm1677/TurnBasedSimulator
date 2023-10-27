#pragma once
#include "AI.h"
#include "../GameSimulator/GameState.h"

class RulesAI :
    public AI
{
public:
    RulesAI(GameState state)
        : m_State(state) {}
    Action GetAction() const override;
private:
    GameState m_State;
    std::vector<Action> getKingThreats(User player) const;
};

