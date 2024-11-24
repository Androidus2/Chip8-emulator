#include "Button.h"


Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const std::function<void()>& onClick) : onClick(onClick)
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1);
	this->text.setFont(font);
	this->text.setCharacterSize(20);
	this->text.setFillColor(sf::Color::Black);
	SetText(text);
}
void Button::Draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}
void Button::Update()
{
	if (shape.getGlobalBounds().contains(mousePositionInUI))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			shape.setFillColor(clickColor);
		}
		else
		{
			shape.setFillColor(hoverColor);
		}
	}
	else
	{
		shape.setFillColor(normalColor);
	}
}
void Button::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !clickUsed)
	{
		if (shape.getGlobalBounds().contains(mousePositionInUI))
		{
			clickUsed = true;
			onClick();
		}
	}
}
void Button::SetText(const std::string& text)
{
	this->text.setString(text);

	// Center text
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f);
}
void Button::SetOnClick(const std::function<void()>& onClick)
{
	this->onClick = onClick;
}
void Button::SetPosition(const sf::Vector2f& position)
{
	shape.setPosition(position);
	SetText(text.getString());
}

void Button::SetNormalColor(const sf::Color& color)
{
	normalColor = color;
}
void Button::SetHoverColor(const sf::Color& color)
{
	hoverColor = color;
}
void Button::SetClickColor(const sf::Color& color)
{
	clickColor = color;
}