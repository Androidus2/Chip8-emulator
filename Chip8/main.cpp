#include <iostream>
#include "SFML\Graphics.hpp"
#include "Chip8.h"
#include "Button.h"
#include "FileSelection.h"

void HandleInput(Chip8& chip8, sf::Event& ev) {
	if (ev.type == sf::Event::KeyPressed) {
		if (ev.key.code == sf::Keyboard::Num1) chip8.SetKey(0x1, 1);
		else if (ev.key.code == sf::Keyboard::Num2) chip8.SetKey(0x2, 1);
		else if (ev.key.code == sf::Keyboard::Num3) chip8.SetKey(0x3, 1);
		else if (ev.key.code == sf::Keyboard::Num4) chip8.SetKey(0xC, 1);

		else if (ev.key.code == sf::Keyboard::Q) chip8.SetKey(0x4, 1);
		else if (ev.key.code == sf::Keyboard::W) chip8.SetKey(0x5, 1);
		else if (ev.key.code == sf::Keyboard::E) chip8.SetKey(0x6, 1);
		else if (ev.key.code == sf::Keyboard::R) chip8.SetKey(0xD, 1);

		else if (ev.key.code == sf::Keyboard::A) chip8.SetKey(0x7, 1);
		else if (ev.key.code == sf::Keyboard::S) chip8.SetKey(0x8, 1);
		else if (ev.key.code == sf::Keyboard::D) chip8.SetKey(0x9, 1);
		else if (ev.key.code == sf::Keyboard::F) chip8.SetKey(0xE, 1);

		else if (ev.key.code == sf::Keyboard::Z) chip8.SetKey(0xA, 1);
		else if (ev.key.code == sf::Keyboard::X) chip8.SetKey(0x0, 1);
		else if (ev.key.code == sf::Keyboard::C) chip8.SetKey(0xB, 1);
		else if (ev.key.code == sf::Keyboard::V) chip8.SetKey(0xF, 1);
	}
	else if (ev.type == sf::Event::KeyReleased) {
		if (ev.key.code == sf::Keyboard::Num1) chip8.SetKey(0x1, 0);
		else if (ev.key.code == sf::Keyboard::Num2) chip8.SetKey(0x2, 0);
		else if (ev.key.code == sf::Keyboard::Num3) chip8.SetKey(0x3, 0);
		else if (ev.key.code == sf::Keyboard::Num4) chip8.SetKey(0xC, 0);

		else if (ev.key.code == sf::Keyboard::Q) chip8.SetKey(0x4, 0);
		else if (ev.key.code == sf::Keyboard::W) chip8.SetKey(0x5, 0);
		else if (ev.key.code == sf::Keyboard::E) chip8.SetKey(0x6, 0);
		else if (ev.key.code == sf::Keyboard::R) chip8.SetKey(0xD, 0);

		else if (ev.key.code == sf::Keyboard::A) chip8.SetKey(0x7, 0);
		else if (ev.key.code == sf::Keyboard::S) chip8.SetKey(0x8, 0);
		else if (ev.key.code == sf::Keyboard::D) chip8.SetKey(0x9, 0);
		else if (ev.key.code == sf::Keyboard::F) chip8.SetKey(0xE, 0);

		else if (ev.key.code == sf::Keyboard::Z) chip8.SetKey(0xA, 0);
		else if (ev.key.code == sf::Keyboard::X) chip8.SetKey(0x0, 0);
		else if (ev.key.code == sf::Keyboard::C) chip8.SetKey(0xB, 0);
		else if (ev.key.code == sf::Keyboard::V) chip8.SetKey(0xF, 0);
	}
}

void CenterChip8Screen(sf::RenderWindow& window, sf::Sprite& chip8Sprite) {
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2u textureSize = chip8Sprite.getTexture()->getSize();
	chip8Sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	chip8Sprite.setPosition(windowSize.x / 2, windowSize.y / 2);
}

bool clickUsed = false;
sf::Font font;
sf::Vector2f mousePositionInUI;

int main()
{
	const int scale = 10;
	float delay = 1.0f / 100.0f;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Chip8 Emulator");
	window.setFramerateLimit(1.0f / delay);

	sf::Texture chip8Texture;
	chip8Texture.create(displayWidth, displayHeight);

	font.loadFromFile("Resources/Roboto-Black.ttf");

	sf::Sprite chip8Sprite;
	chip8Sprite.setTexture(chip8Texture);
	chip8Sprite.setScale(scale, scale);
	chip8Sprite.setPosition(5, 60);

	CenterChip8Screen(window, chip8Sprite);

	Chip8 chip8;
	chip8.LoadROM("Roms/Tetris.ch8");

	uint32_t display[displayHeight * displayWidth];

	Button loadButton(sf::Vector2f(120, 50), sf::Vector2f(5, 5), "Load ROM",
		[&chip8]() {
			std::string filename = OpenFileDialog();
			if (filename != "") chip8.LoadROM(filename);
		});

	bool isPaused = false;

	Button pauseButton(sf::Vector2f(120, 50), sf::Vector2f(135, 5), "Pause / Play",
		[&isPaused]() {
			isPaused = !isPaused;
		});

	while (window.isOpen())
	{
		clickUsed = false;
		mousePositionInUI = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				CenterChip8Screen(window, chip8Sprite);
			}

			if (!isPaused)
				HandleInput(chip8, event);
			loadButton.HandleEvent(event);
			pauseButton.HandleEvent(event);
		}

		loadButton.Update();
		pauseButton.Update();

		if (!isPaused) {
			chip8.Cycle();
			chip8.GetDisplay(display);

			chip8Texture.update((sf::Uint8*)display, displayWidth, displayHeight, 0, 0);
		}

		window.clear();
		window.draw(chip8Sprite);
		loadButton.Draw(window);
		pauseButton.Draw(window);
		window.display();
	}
	return 0;
}
