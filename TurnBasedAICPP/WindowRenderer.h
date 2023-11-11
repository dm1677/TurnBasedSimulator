#pragma once
#include "Window.h"
#include "GameSimulator/GameState.h"

class WindowRenderer
{
public:
	WindowRenderer() {}
	~WindowRenderer() { delete m_Window; }

	void RenderState(const GameState& state);
	void OnFrame() const { m_Window->Run(); }
private:
	Window* m_Window = new Window(15*32, 15*32, "Turn Based Simulator");
};

