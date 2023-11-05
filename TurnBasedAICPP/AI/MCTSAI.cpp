#include "MCTSAI.h"
#include "../Enums.h"
#include <chrono>
#include <random>

static const int c_Iterations = 100;
Action MCTSAI::GetAction() const
{
    Node* root = new Node(m_State, Action());
    
    for (int i = 0; i < c_Iterations; i++)
    {
        Node* node = root;

        while (node->IsFullyExpanded() && !node->GetState().IsGameOver())
        {
            node = node->GetBestChild(sqrt(2));
        }

        if (!node->IsFullyExpanded() && !node->GetState().IsGameOver())
        {
            node = node->Expand();
        }

        auto result = simulate(node->GetState());

        backpropagate(node, result);
    }

    Node* bestNode = root->GetBestChild(0.0);
    Action bestMove = bestNode->GetAction();
    delete root;
    return bestMove;
}

int MCTSAI::simulate(const GameState& state) const
{
    //auto start = std::chrono::high_resolution_clock::now();
    
    //-------------------------------------------------------------------------------
    static int shallow = 0;
    int i = 0;
    Simulator simulator(state, Action());
    while(!simulator.GetCurrentState().IsGameOver() && i < 3000)
    {
        std::vector<Action> actions = simulator.GetCurrentState().GetLegalMoves();
        simulator.GenerateNewState(GetRandomAction(actions));
        i++;
    }

    //-------------------------------------------------------------------------------

    //auto stop = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    //std::cout << "Simulation performed " << i << " actions in " << duration.count() << " milliseconds." << std::endl;
    if (i < 3000)
    {
        shallow++;
        std::cout << "\nShallow tree #" << shallow << std::endl;
    }

    const auto& finalState = simulator.GetCurrentState();
    if (finalState.IsGameOver())
    {
        if (m_State.GetPlayer() == finalState.GetResult())
            return 1;
        else return -1;
    }
    else
        return getEvaluation(m_State.GetPlayer(), finalState);
}

int MCTSAI::getEvaluation(User player, const GameState& state) const
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
    auto diff = playerKingHealthPool - enemyKingHealthPool;
    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;
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

Action MCTSAI::GetRandomAction(const std::vector<Action>& actions)
{
    if (actions.empty()) return Action();

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, actions.size() - 1);

    int randomNumber = dist(rng);
    return actions[randomNumber];
}
