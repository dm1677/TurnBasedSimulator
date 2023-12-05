#pragma once
#include "AI.h"
#include "../GameSimulator/GameState.h"

namespace TurnBasedSimulator
{
    class RulesAI :
        public AI
    {
    public:
        RulesAI(GameState state)
            : m_State(state) {}
        Action GetAction() const override;
    private:
        GameState m_State;
        std::vector<Action> getKingThreats(User player, const GameState& state) const;
        Action getSafestMove(const std::vector<Action>& actions) const;
    };
}