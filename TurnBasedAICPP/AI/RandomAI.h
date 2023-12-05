#pragma once
#include "AI.h"
#include "../GameSimulator/GameState.h"

namespace TurnBasedSimulator
{
    class RandomAI :
        public AI
    {
    public:
        explicit RandomAI(const GameState& state, bool useHeuristic = false)
            : m_State(state), m_UseHeuristic(useHeuristic) {};
        Action GetAction() const override;




    private:
        const GameState& m_State;
        const bool m_UseHeuristic;

        Action getSemirandomAction() const;

        ActionCategory categoriseAction(const Action& action) const;
        bool isUnitThreatened(const Unit& king) const;
        bool wouldThreatenKing(Pos position, UnitType unitType) const;
    };
}