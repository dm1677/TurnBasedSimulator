#include "MCTSAI.h"
#include "RandomAI.h"
#include "../Enums.h"
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <algorithm>

std::mutex nodeMutex;

static const int c_Iterations = 2500;
static const int c_Depth = 20;

Action MCTSAI::GetAction() const
{
    if (!m_TreeSearch)
    {
        auto moves = m_State.GetLegalMoves();
        auto scores = std::vector<double>(moves.size(), 0.0);

        for (int j = 0; j < 5; j++)
        {
            for (int i = 0; i < moves.size(); i++)
            {
                Simulator simulator(m_State, moves[i]);
                scores[i] += simulate(simulator.GenerateNewState(moves[i]), c_Depth);
            }
        }

        auto largest = std::max_element(scores.begin(), scores.end());
        int index = std::distance(scores.begin(), largest);
        return moves[index];
    }

    Node* root = new Node(m_State, Action());
    
    for (int i = 0; i < c_Iterations; i++)
    {
        Node* node = root;

        while (node->IsFullyExpanded() && !node->GetState().IsGameOver())
        {
            if (m_State.GetPlayer() == node->GetState().GetPlayer())
                node = node->GetBestChild(sqrt(2));
            else
                node = node->GetWorstChild(sqrt(2));
        }

        if (!node->IsFullyExpanded() && !node->GetState().IsGameOver())
        {
            node = node->Expand();
        }

        std::vector<std::thread> threads;
        for (int j = 0; j < std::thread::hardware_concurrency(); j++)
            threads.emplace_back(&MCTSAI::simAndBackprop, this, node);

        for (auto& thread : threads)
            thread.join();
    }

    const Node* bestNode = root->GetBestChild(0.0);
    Action bestMove = bestNode->GetAction();
    std::cout << "Cumulative evaluation: " << root->GetWins() << std::endl;
    std::cout << "Best node: " << bestNode->GetWins() << " score, " << bestNode->GetVisits() << "/" << root->GetVisits() << " visits." << std::endl;
    delete root;
    return bestMove;
}

double MCTSAI::simulate(const GameState& state, int depth) const
{
    int i = 0;
    Simulator simulator(state, Action());
    while(!simulator.GetCurrentState().IsGameOver() && i < depth)
    {
        std::vector<Action> actions = simulator.GetCurrentState().GetLegalMoves();
        simulator.GenerateNewState(RandomAI(simulator.GetCurrentState(), false).GetAction());
        i++;
    }

    const auto& finalState = simulator.GetCurrentState();
    if (finalState.IsGameOver())
    {
        if (m_State.GetPlayer() == finalState.GetResult())
            return 1000;
        else return -1000;
    }
    else
        return getEvaluation(m_State.GetPlayer(), finalState);
}

double MCTSAI::getEvaluation(User player, const GameState& state) const
{
    int playerKingHealthPool = 0;
    int enemyKingHealthPool = 0;
    const auto& kings = state.GetKings();

    for (const auto& king : kings)
    {
        if (king.GetOwner() == player)
            playerKingHealthPool += king.GetHealth();
        else if (king.GetOwner() != Neutral)
            enemyKingHealthPool += king.GetHealth();
    }

    return (playerKingHealthPool - enemyKingHealthPool) / 100.0;
}

void MCTSAI::backpropagate(Node* node, double result) const
{
    while (node)
    {
        node->IncrementVisits();
        node->UpdateWins(result);
        node = node->GetParent();
    }
}


void MCTSAI::safeBackpropagate(Node* node, double result) const
{
    std::lock_guard<std::mutex> lock(nodeMutex);
    backpropagate(node, result);
}

Action MCTSAI::GetRandomAction(const std::vector<Action>& actions)
{
    if (actions.empty()) return Action();

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, actions.size() - 1);

    int randomNumber = dist(rng);
    return actions[randomNumber];
}

void MCTSAI::simAndBackprop(Node* node) const
{
    safeBackpropagate(node, simulate(node->GetState(), c_Depth));
}