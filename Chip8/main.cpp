#include <iostream>
#include "SFML\Graphics.hpp"
#include "Chip8.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Chip8 Emulator");

	const int scale = 10;

	sf::Texture chip8Texture;
	chip8Texture.create(displayWidth, displayHeight);


	sf::Sprite chip8Sprite;
	chip8Sprite.setTexture(chip8Texture);
	chip8Sprite.setScale(scale, scale);

	Chip8 chip8;
	chip8.LoadROM("Roms/Tetris.ch8");

	uint32_t display[displayHeight * displayWidth];

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}
		chip8.Cycle();
		chip8.GetDisplay(display);
		
		chip8Texture.update((sf::Uint8*)display, displayWidth, displayHeight, 0, 0);

		window.clear();
		window.draw(chip8Sprite);
		window.display();
	}
	return 0;
}
