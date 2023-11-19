#pragma once
#include <SFML/Graphics.hpp>
class Window
{
public:
	Window(int width, int height, const std::string& title);
	~Window() { delete m_Window; delete m_SpriteSheet; delete m_BackgroundTexture; }

	void Run();
	unsigned int GetWidth() const { return m_Window->getSize().x; }
	unsigned int GetHeight() const { return m_Window->getSize().y; }
	bool IsOpen() const { return m_Window->isOpen(); }
	void AddSprite(int x, int y, int sprite, int color);
	void FlushSprites() { m_Sprites.clear(); }
	void Close() const { m_Window->close(); }
	bool GetRightKeyPressed() const { return m_RightPressed; }
private:
	sf::RenderWindow* m_Window;
	sf::Texture* m_SpriteSheet = new sf::Texture;
	sf::Texture* m_BackgroundTexture = new sf::Texture;
	sf::Sprite m_BackgroundSprite;
	std::vector<sf::Sprite> m_Sprites;

	bool m_RightPressed = false;
};

