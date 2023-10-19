#include "GameState.h"

bool GameState::passable(int x, int y) const {
	for (Unit unit : units) {
		if (unit.GetX()  == x && unit.GetY() == y) {
			return false;
		}
	}
	return true;
}

bool GameState::game_over()
{
	if (units[4].getUnitType() == King) { return false; }
	if (units[3].getUnitType() != King) { return true; }
	if (units[2].getOwner() != units[3].getOwner()) { return false; }

	return true;
}

int GameState::result() {
	if (game_over())
	{
		return units[2].getOwner();
	}

	return -1;
}

User GameState::GetEnemy() const {
	if (player_to_move == Player) { return Enemy; }
	if (player_to_move == Enemy) { return Player; }
}

std::vector<Action> GameState::GetLegalMoves() {

	std::vector<Action> moves;
	int money = GetMoney(player_to_move);
	
	if (money >= 3)
	{
		for (int y = 0; y < 15; y++)
		{
			for (int x = 0; x < 15; x++)
			{
				if (passable(x, y)) {
					Action action(Gobbo, x, y);
					moves.push_back(action);
					if (money >= 5) {
						Action action(Prawn, x, y);
						moves.push_back(action);
					}
					if (money >= 8) {
						Action action(Building, x, y);
						moves.push_back(action);
					}
					if (money >= 12) {
						Action action(Knight, x, y);
						moves.push_back(action);
					}
				}
			}
		}
	}
	
	for (uint32_t i = 0; i < units.size(); i++) {
		if (units[i].getOwner() != (int)player_to_move) { continue; }
		if (units[i].getUnitType() == (int)Resource) { continue; }
		auto movement = get_movement(units[i]);
		for (auto m : movement)
		{
			Action action(i, m.first, m.second);			
			moves.push_back(action);
		}

		auto attacks = get_attacks(units[i]);
		for (auto a : attacks)
		{
			Action action(i, a);
			moves.push_back(action);
		}
	}
	
	return moves;
}

std::vector<std::pair<int, int>> GameState::get_movement(const Unit &unit)
{
	int speed = unit.get_speed();
	std::vector<std::pair<int, int>> moves;

	if (speed == 0) { return moves; }

	std::pair<int, int> current_position(unit.GetX(), unit.GetY());

	const std::vector<std::pair<int, int>> cardinal_vectors = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	const std::vector<std::pair<int, int>> diagonal_vectors = { { 1, -1 }, { 1, 1 }, { -1, 1 }, { -1, -1 } };

	std::vector<std::pair<int, int>> vectors;

	switch (unit.get_direction()) {
	case Line:
		vectors = cardinal_vectors;
		break;
	case Diagonal:
		vectors = diagonal_vectors;
		break;
	case LineAndDiagonal:
		vectors = cardinal_vectors;
		vectors.insert(vectors.end(), diagonal_vectors.begin(), diagonal_vectors.end());
		break;
	}
	int x, y;

	for (uint32_t k = 0; k < vectors.size(); k++)
	{
		for (int i = 1; i <= speed; i++)
		{
			x = current_position.first + (i * vectors[k].first);
			y = current_position.second + (i * vectors[k].second);

			if (!IsInBounds(x, y) || !passable(x, y)) { break; }

			moves.push_back(std::make_pair(x, y));
		}
	}

	return moves;
}

std::vector<int> GameState::get_attacks(const Unit &unit) {

	std::vector<int> enemy_units;
	std::vector<int> attack_actions;

	for (uint32_t i = 2; i < units.size(); i++)
	{
		if (units[i].getOwner() != Neutral && units[i].getOwner() != player_to_move)
		{
			enemy_units.push_back(i);
		}
	}

	const std::vector<std::pair<int, int>> cardinal_vectors = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	const std::vector<std::pair<int, int>> diagonal_vectors = { { 1, -1 }, { 1, 1 }, { -1, 1 }, { -1, -1 } };

	std::vector<std::pair<int, int>> vectors;

	switch (unit.get_direction()) {
	case Line:
		vectors = cardinal_vectors;
		break;
	case Diagonal:
		vectors = diagonal_vectors;
		break;
	case LineAndDiagonal:
		vectors = cardinal_vectors;
		vectors.insert(vectors.end(), diagonal_vectors.begin(), diagonal_vectors.end());
		break;
	}
	int x, y;

	for (uint32_t k = 0; k < vectors.size(); k++)
	{
		for (int i = 1; i <= unit.get_range(); i++)
		{
			x = unit.GetX() + (i * vectors[k].first);
			y = unit.GetY() + (i * vectors[k].second);

			if (!IsInBounds(x, y) || !passable(x, y)) { break; }

			for (uint32_t j = 1; j < enemy_units.size(); j++)
			{
				if (units[j].GetX() == x && units[j].GetY() == y) {
					attack_actions.push_back(j);
				}
			}
		}
	}

	return attack_actions;
}

bool GameState::IsInBounds(int x, int y)
{
	if (x < 0
		|| y < 0
		|| y > 14
		|| x > 14
		)
		return false;

	else return true;
}

void GameState::print_units() {
	for (auto unit : units)
	{
		unit.print();
		std::cout << std::endl;
	}
}

void GameState::DrawGrid() {
	char grid[15][15];

	std::cout << std::endl;

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			grid[x][y] = '-';
		}
	}

	for (auto unit : units)
	{
		grid[unit.GetX()][unit.GetY()] = unit.get_char_representation();
	}

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			std::cout << grid[x][y];
		}
		std::cout << std::endl;
	}
}

void GameState::create_units()
{
	Unit add_unit(15, 15, 8, (int)Resource, (int)Player);
	Unit add_unit2(15, 15, 8, (int)Resource, (int)Enemy);
	units.push_back(add_unit);
	units.push_back(add_unit2);

	Unit a(0, 0, 40, (int)King, (int)Player);
	Unit b(0, 14, 40, (int)King, (int)Player);
	Unit c(14, 0, 40, (int)King, (int)Enemy);
	Unit d(14, 14, 40, (int)King, (int)Enemy);
	units.push_back(a);
	units.push_back(b);
	units.push_back(c);
	units.push_back(d);

	std::vector<std::pair<int, int>> vectors = { { 7, 0 }, { 0, 7 }, { 14, 7 }, { 7, 14 }, { 6,6 }, { 8,8 }, { 6,8 }, { 8,6 } };

	for (auto pair : vectors) {
		Unit u(pair.first, pair.second, 0, (int)Tree, (int)Neutral);
		units.push_back(u);
	}
}

int GameState::GetMoney(User user) const
{
	return (user == (User)units[0].getOwner()) ? units[0].getHealth() : units[1].getHealth();
}