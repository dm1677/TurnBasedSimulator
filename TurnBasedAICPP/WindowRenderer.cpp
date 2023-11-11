#include "WindowRenderer.h"

void WindowRenderer::RenderState(const GameState& state)
{
	m_Window->FlushSprites();
	for (const auto& unit : state.GetUnitData())
	{
		int spr = 0;
		switch (unit.GetUnitType())
		{
		case Tree:
			spr = 1;
			break;
		case King:
			spr = 2;
			break;
		case Gobbo:
			spr = 3;
			break;
		case Prawn:
			spr = 4;
			break;
		case Building:
			spr = 5;
			break;
		case Knight:
			spr = 6;
			break;
		case Resource:
			continue;
		default:
			break;			
		}

		int color;
		switch (unit.GetOwner())
		{
		case Player:
			color = 1;
			break;
		case Enemy:
			color = 2;
			break;
		case Neutral:
			color = 0;
			break;
		default:
			color = 0;
			break;
		}
		m_Window->AddSprite(unit.GetX(), unit.GetY(), spr, color);
	}
}