#include "Window.h"

Window::Window(int width, int height, const std::string& title)
{
    m_Window = new sf::RenderWindow(sf::VideoMode(width, height), title);
    m_Window->setFramerateLimit(60);

    m_SpriteSheet->loadFromFile("assets/spritesheet.png");

    m_BackgroundTexture->loadFromFile("assets/sandstone.png");
    m_BackgroundTexture->setRepeated(true);
    m_BackgroundSprite.setTexture(*m_BackgroundTexture);
    m_BackgroundSprite.setTextureRect(sf::IntRect(0, 0, 15 * 32, 15 * 32));

    m_Sprites.reserve(250);
}

void Window::Run() const
{
    sf::Event event;
    while (m_Window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_Window->close();
    }

    m_Window->clear();
    m_Window->draw(m_BackgroundSprite);

    for (const auto& sprite : m_Sprites)
    {
        m_Window->draw(sprite);
    }

    m_Window->display();
}

void Window::AddSprite(int x, int y, int sprite, int color)
{
    sf::IntRect subTexturePosition((sprite % 3) * 32, (sprite / 3) * 32, 32, 32);
    m_Sprites.emplace_back(*m_SpriteSheet, subTexturePosition);
    m_Sprites.back().setPosition(x * 32, y * 32);
    if (color == 1)
        m_Sprites.back().setColor(sf::Color::Green);
    if (color == 2)
        m_Sprites.back().setColor(sf::Color::Red);    
}
