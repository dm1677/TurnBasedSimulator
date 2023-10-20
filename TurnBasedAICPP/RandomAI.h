#pragma once
#include "AI.h"

class RandomAI :
    public AI
{
public:
    explicit RandomAI(const GameState& state)
        : m_State(state) {};
    Action GetAction() const override;
private:
    const GameState& m_State;
};

