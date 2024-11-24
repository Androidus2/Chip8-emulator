#include <iostream>
#include "SFML\Graphics.hpp"
#include "Chip8.h"
#include "Button.h"
#include "FileSelection.h"
#include "InputField.h"

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
	float scale = 10;
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
			std::string filename = OpenFileDialog(true);
			if (filename != "") chip8.LoadROM(filename);
		});

	bool isPaused = false;
	bool step = false;

	Button pauseButton(sf::Vector2f(120, 50), sf::Vector2f(135, 5), "Pause / Play",
		[&isPaused]() {
			isPaused = !isPaused;
		});


	Button stepButton(sf::Vector2f(50, 50), sf::Vector2f(265, 5), "Step",
		[&step, &isPaused]() {
			if(isPaused)
				step = true;
		});


	InputField scaleField(sf::Vector2f(120, 30), sf::Vector2f(400, 15), "10");
	scaleField.SetOnFinishEdit([&scale, &chip8Sprite, &window, &scaleField](const std::string& text) {
		try {
			std::stof(text);
		}
		catch (std::exception e) {
			scaleField.SetText(std::to_string(scale));
			return;
		}
		scale = std::stof(text);
		chip8Sprite.setScale(scale, scale);
		CenterChip8Screen(window, chip8Sprite);
		});

	sf::Text scaleText;
	scaleText.setFont(font);
	scaleText.setCharacterSize(20);
	scaleText.setFillColor(sf::Color::White);
	scaleText.setString("Scale:");
	scaleText.setPosition(365, 25);
	scaleText.setOrigin(scaleText.getLocalBounds().width / 2, scaleText.getLocalBounds().height / 2);


	InputField delayField(sf::Vector2f(120, 30), sf::Vector2f(600, 15), "100");
	delayField.SetOnFinishEdit([&delay, &window, &delayField](const std::string& text) {
		try {
			float val = std::stof(text);
			if (val <= 0) throw std::exception();
		}
		catch (std::exception e) {
			delayField.SetText(std::to_string(1.0f / delay));
			return;
		}
		delay = 1.0f / std::stof(text);
		window.setFramerateLimit(1.0f / delay);
		});

	sf::Text delayText;
	delayText.setFont(font);
	delayText.setCharacterSize(20);
	delayText.setFillColor(sf::Color::White);
	delayText.setString("Delay:");
	delayText.setPosition(560, 25);
	delayText.setOrigin(delayText.getLocalBounds().width / 2, delayText.getLocalBounds().height / 2);



	Button saveStateButton(sf::Vector2f(120, 50), sf::Vector2f(5, 60), "Save State",
		[&chip8]() {
			std::string filename = SaveFileDialog();
			if (filename != "") chip8.SaveState(filename);
		});

	Button loadStateButton(sf::Vector2f(120, 50), sf::Vector2f(135, 60), "Load State",
		[&chip8]() {
			std::string filename = OpenFileDialog(false);
			if (filename != "") chip8.LoadState(filename);
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

			if (!isPaused || step)
				HandleInput(chip8, event);
			loadButton.HandleEvent(event);
			pauseButton.HandleEvent(event);
			scaleField.HandleEvent(event);
			delayField.HandleEvent(event);
			stepButton.HandleEvent(event);
			saveStateButton.HandleEvent(event);
			loadStateButton.HandleEvent(event);
		}

		loadButton.Update();
		pauseButton.Update();
		stepButton.Update();
		saveStateButton.Update();
		loadStateButton.Update();

		if (!isPaused) {
			chip8.Cycle();
			chip8.GetDisplay(display);

			chip8Texture.update((sf::Uint8*)display, displayWidth, displayHeight, 0, 0);
		}
		else if (step) {
			std::cout << "Step" << std::endl;
			chip8.Cycle();
			chip8.GetDisplay(display);

			chip8Texture.update((sf::Uint8*)display, displayWidth, displayHeight, 0, 0);
			step = false;
		}

		window.clear();
		window.draw(chip8Sprite);
		stepButton.Draw(window);
		delayField.Draw(window);
		window.draw(delayText);
		scaleField.Draw(window);
		window.draw(scaleText);
		pauseButton.Draw(window);
		loadButton.Draw(window);
		saveStateButton.Draw(window);
		loadStateButton.Draw(window);
		window.display();
	}
	return 0;
}
