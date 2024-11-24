#include "InputField.h"

InputField::InputField(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text)
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

	isSelected = false;
}
void InputField::Draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}
void InputField::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (shape.getGlobalBounds().contains(mousePositionInUI) && !clickUsed)
		{
			clickUsed = true;
			isSelected = true;
			shape.setOutlineColor(sf::Color::Green);
		}
		else if (isSelected)
		{
			isSelected = false;
			shape.setOutlineColor(sf::Color::Black);
			if (onFinishEdit != nullptr)
				onFinishEdit(text.getString());
		}
	}
	else if (event.type == sf::Event::TextEntered && isSelected)
	{
		if (event.text.unicode == 8 && text.getString().getSize() > 0)
		{
			std::string currentText = text.getString();
			currentText.pop_back();
			text.setString(currentText);
		}
		else if (event.text.unicode >= 32 && event.text.unicode <= 126)
		{
			std::string currentText = text.getString();
			currentText += event.text.unicode;
			text.setString(currentText);
		}
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && isSelected)
	{
		isSelected = false;
		shape.setOutlineColor(sf::Color::Black);
		if (onFinishEdit != nullptr)
			onFinishEdit(text.getString());
	}
}
void InputField::SetText(const std::string& text)
{
	this->text.setString(text);
	// Center text
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f);
}
const sf::Text& InputField::GetText() const
{
	return text;
}
void InputField::SetOnFinishEdit(const std::function<void(const std::string&)>& onFinishEdit)
{
	this->onFinishEdit = onFinishEdit;
}
void InputField::SetPosition(const sf::Vector2f& position)
{
	shape.setPosition(position);
	text.setPosition(position.x + 6, position.y + 6);
}
std::string InputField::GetValue() const {
	return text.getString();
}