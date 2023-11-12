#pragma once
#include "AI.h"
#include "../GameSimulator/GameState.h"

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
};

