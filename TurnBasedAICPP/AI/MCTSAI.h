#pragma once
#include "AI.h"
#include "../GameSimulator/GameState.h"
#include "../GameSimulator/Simulator.h"
#include <limits>

class MCTSAI :
    public AI
{
public:
    MCTSAI(const GameState& state)
        : m_State(state) {}
    Action GetAction() const override;
    static Action GetRandomAction(const std::vector<Action>& actions);
private:
    GameState m_State;
    class Node;
    int simulate(const GameState& state) const;
    void backpropagate(Node* node, double result) const;
    int getEvaluation(User player, const GameState& state) const;

    class Node
    {
    public:
        Node(const GameState& state, const Action& action, Node* parent = nullptr)
            : m_State(state), m_Action(action), m_Parent(parent)
        {
            if (!state.IsGameOver()) m_UntriedMoves = state.GetLegalMoves();
        }

        ~Node() { for (auto child : m_Children) delete child; }

        bool IsFullyExpanded() const { return m_UntriedMoves.empty(); }
        Node* GetBestChild(double explorationConstant) const
        {
            Node* best = nullptr;
            double bestValue = std::numeric_limits<double>::lowest();

            for (auto child : m_Children)
            {
                auto uct = getUCTScore(child, explorationConstant);
                if (uct > bestValue)
                {
                    bestValue = uct;
                    best = child;
                }
            }
            return best;
        }
        Node* Expand()
        {
            Action action = GetRandomAction(m_UntriedMoves);
            m_UntriedMoves.erase(std::remove(m_UntriedMoves.begin(), m_UntriedMoves.end(), action)); //TODO: figure out a better solution
            Simulator simulator(m_State, action);
            auto newState = simulator.GenerateNewState(action);
            auto childNode = new Node(newState, action, this);
            m_Children.push_back(childNode);
            return childNode;
        }
        int GetWins() const { return m_Wins; }
        int GetVisits() const { return m_Visits; }
        void IncrementVisits() { m_Visits++; }
        void UpdateWins(int wins) { m_Wins += wins; }
        Node* GetParent() const { return m_Parent; }
        const GameState& GetState() const { return m_State; }
        Action GetAction() const { return m_Action; }
    private:
        GameState m_State;
        Action m_Action;
        Node* m_Parent;
        std::vector<Node*> m_Children;
        int m_Visits = 0;
        int m_Wins = 0;
        std::vector<Action> m_UntriedMoves;

        inline double getUCTScore(const Node* node, double c) const
        {
            return (node->GetWins() / (node->GetVisits() + 1e-6))
                + (c * std::sqrt(std::log(m_Visits + 1) / node->GetVisits() + 1e-6));
        }
    };
};

