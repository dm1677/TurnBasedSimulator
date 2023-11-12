#include "MCTSAI.h"
#include "../Enums.h"
#include <chrono>
#include <random>
#include <thread>
#include <mutex>

std::mutex nodeMutex;

static const int c_Iterations = 10000;
Action MCTSAI::GetAction() const
{
    Node* root = new Node(m_State, Action());
    
    for (int i = 0; i < c_Iterations; i++)
    {
        /*if (i % 50 == 0)
            std::cout << "\nIteration " << i << "/" << c_Iterations << std::endl;*/
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

        std::thread t1(&MCTSAI::simAndBackprop, this, node);
        std::thread t2(&MCTSAI::simAndBackprop, this, node);
        std::thread t3(&MCTSAI::simAndBackprop, this, node);
        std::thread t4(&MCTSAI::simAndBackprop, this, node);

        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }

    const Node* bestNode = root->GetBestChild(0.0);
    Action bestMove = bestNode->GetAction();
    std::cout << "Last state: " << root->GetWins() << " score, " << root->GetVisits() << " visits." << std::endl;
    std::cout << "Best node: " << bestNode->GetWins() << " score, " << bestNode->GetVisits() << " visits." << std::endl;
    delete root;
    return bestMove;
}

double MCTSAI::simulate(const GameState& state) const
{
    //auto start = std::chrono::high_resolution_clock::now();
    
    //-------------------------------------------------------------------------------
    //static int shallow = 0;
    int i = 0;
    int depth = 50;
    Simulator simulator(state, Action());
    while(!simulator.GetCurrentState().IsGameOver() && i < depth)
    {
        std::vector<Action> actions = simulator.GetCurrentState().GetLegalMoves();
        simulator.GenerateNewState(GetRandomAction(actions));
        i++;
    }

    //-------------------------------------------------------------------------------

    //auto stop = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    //std::cout << "Simulation performed " << i << " actions in " << duration.count() << " milliseconds." << std::endl;

    //if (i < depth)
    //{
    //    shallow++;
    //    std::cout << "\nShallow tree #" << shallow  << " - Depth: " << i << std::endl;
    //}

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
    //if (diff > 0) return 1;
    //if (diff < 0) return -1;
    //return 0;
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
    safeBackpropagate(node, simulate(node->GetState()));
}