#include "SFML\Graphics.hpp"
#include <iostream>
#include <stdexcept>

using namespace sf;

Texture texture;
Sprite sprite;
void Load()
{
	if (!texture.loadFromFile("res/img/spaceship1.png")) {
		throw std::invalid_argument("Loading error!");
	}
}

void Update()
{
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	Vector2f move;
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		move.x--;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		move.x++;
	}
	sprite.move(move*300.0f*dt);
}

void Render(RenderWindow &window) { window.draw(sprite); }

int main() {
	RenderWindow window(VideoMode(400, 400), "SFML works!");

	try {
		Load();
	}
	catch (const std::exception &) {
		std::cerr << "Load error" << std::endl;
		return 1;
	}

	sprite.setTexture(texture);
	sprite.setScale(Vector2f(2.f, 2.f));

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		window.clear();
		Update();
		Render(window);
		window.display();
	}

	return 0;
}