#pragma once

#include "SFML/Graphics.hpp"
#include <functional>
#include <iostream>

extern bool clickUsed;
extern sf::Font font;
extern sf::Vector2f mousePositionInUI;

class Button
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	std::function<void()> onClick;

	sf::Color normalColor = sf::Color::White;
	sf::Color hoverColor = sf::Color(200, 200, 200);
	sf::Color clickColor = sf::Color(150, 150, 150);
public:
	Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const std::function<void()>& onClick);
	void Draw(sf::RenderWindow& window) const;
	void Update();
	void HandleEvent(const sf::Event& event);
	void SetText(const std::string& text);
	void SetOnClick(const std::function<void()>& onClick);
	void SetPosition(const sf::Vector2f& position);

	void SetNormalColor(const sf::Color& color);
	void SetHoverColor(const sf::Color& color);
	void SetClickColor(const sf::Color& color);
};