#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"

#include <iostream>
#include <stdexcept>
#include <Windows.h>
#include <Mmsystem.h>


using namespace sf;

Texture texture;
Sprite sprite;

void Sounds(int swi)
{
	sf::SoundBuffer buffer;
	switch (swi)
	{
		case 0:
			if (!buffer.loadFromFile("res/img/047.wav"))
			{
				throw std::invalid_argument("Loading error!");
				break;
			}
	}
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	return;
}
void Load()
{
	if (!texture.loadFromFile("res/img/spaceship1.png"))
	{
		throw std::invalid_argument("Loading error!");
	}
}

void Update()
{

	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	Vector2f move;
	if (Keyboard::isKeyPressed(Keyboard::Left)) 
	{
		//std::cout << "\a\a\a\a\a\a\a\a";
		move.x--;
		//
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		Sounds(0);
		move.x++;
		
	}
	sprite.move(move*300.0f*dt);
}

void Render(RenderWindow &window) { window.draw(sprite); }

int main() 
{
	RenderWindow window(VideoMode(400, 400), "SFML works!");

	try
	{
		Load();

	}
	catch (const std::exception &) {
		std::cerr << "Load error" << std::endl;
		return 1;
	}

	sprite.setTexture(texture);
	sprite.setScale(Vector2f(2.f, 2.f));

	while (window.isOpen()) 
	{
		Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
		{
			window.close();
		}

		window.clear();
		Update();
		Render(window);
		window.display();
	}

	return 0;
}
