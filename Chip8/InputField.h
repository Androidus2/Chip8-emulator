#pragma once

#include "SFML/Graphics.hpp"
#include <functional>
#include <iostream>

extern bool clickUsed;
extern sf::Font font;
extern sf::Vector2f mousePositionInUI;

class InputField
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	bool isSelected;
	std::function<void(const std::string&)> onFinishEdit;
public:
	InputField(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text);
	void Draw(sf::RenderWindow& window) const;
	void HandleEvent(const sf::Event& event);
	void SetText(const std::string& text);
	const sf::Text& GetText() const;
	void SetOnFinishEdit(const std::function<void(const std::string&)>& onFinishEdit);
	void SetPosition(const sf::Vector2f& position);
	std::string GetValue() const;
};