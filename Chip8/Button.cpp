#include "Button.h"

using namespace std;
using namespace sf;

Button::Button(const Vector2f& size, const Vector2f& position, const string& text, const function<void()>& onClick) : onClick(onClick)
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(Color::White);
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(1);
	this->text.setFont(font);
	this->text.setCharacterSize(20);
	this->text.setFillColor(Color::Black);
	SetText(text);
}
void Button::Draw(RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}
void Button::Update()
{
	if (shape.getGlobalBounds().contains(mousePositionInUI))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
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
void Button::HandleEvent(const Event& event)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && !clickUsed)
	{
		if (shape.getGlobalBounds().contains(mousePositionInUI))
		{
			clickUsed = true;
			onClick();
		}
	}
}
void Button::SetText(const string& text)
{
	this->text.setString(text);

	// Center text
	FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f);
}
void Button::SetOnClick(const function<void()>& onClick)
{
	this->onClick = onClick;
}
void Button::SetPosition(const Vector2f& position)
{
	shape.setPosition(position);
	SetText(text.getString());
}

void Button::SetNormalColor(const Color& color)
{
	normalColor = color;
}
void Button::SetHoverColor(const Color& color)
{
	hoverColor = color;
}
void Button::SetClickColor(const Color& color)
{
	clickColor = color;
}