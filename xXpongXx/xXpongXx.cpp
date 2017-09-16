#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"

#include <iostream>
#include <stdexcept>
#include <Windows.h>
#include <Mmsystem.h>


using namespace sf;

Texture texture;
Sprite sprite;

void LoadMenu() 
{

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
		/*Sounds(0);*/
		move.x++;

	}
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		//std::cout << "\a\a\a\a\a\a\a\a";
		move.y--;
		//
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		/*Sounds(0);*/
		move.y++;

	}
	sprite.move(move*300.0f*dt);
}
void Render(RenderWindow &window) { window.draw(sprite); }
//sf::SoundBuffer buffer;
//void Sounds(int swi)
//{
//	switch (swi)
//	{
//		case 0:
//			if (!buffer.loadFromFile("res/img/0477.wav"))
//			{
//				throw std::invalid_argument("Loading error!");
//			}
//	}
//	sf::Sound sound;
//	sound.setBuffer(buffer);
//	sound.play();
//	return;
//}
class cScreen 
{
	public:
		virtual int Run(sf::RenderWindow &window) = 0;
};

class screen_0 : public cScreen
{
	private:
		int alpha_max;
		int alpha_div;
		bool playing;
	public:
		screen_0(void);
		virtual int Run(sf::RenderWindow &window);
};

screen_0::screen_0(void)
{
	alpha_max = 3 * 255;
	alpha_div = 3;
	playing = false;
}

int screen_0::Run(sf::RenderWindow &window)
{
	try
	{
		LoadMenu();
	}
	catch (const std::exception &) {
		std::cerr << "Load error" << std::endl;
		return 1;
	}



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
		if (Keyboard::isKeyPressed(Keyboard::Num1))
		{
			window.setSize(Vector2u(1080, 600));
			window.setPosition(Vector2i(0, 0));
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2))
		{
			window.setSize(Vector2u(400, 400));
			window.setPosition(Vector2i(760, 340));
		}
		if (Keyboard::isKeyPressed(Keyboard::Num3))
		{
			window.setSize(Vector2u(1920, 1200));
			window.setPosition(Vector2i(0, 0));
		}
		window.clear();
		Render(window);
		window.display();
	}

}

class screen_1 : public cScreen
{
	private:
		float movement_step;
		float posx;
		float posy;
		sf::RectangleShape Rectangle;
	public:
		screen_1(void);
		virtual int Run(sf::RenderWindow &App);
};

screen_1::screen_1(void)
{
	movement_step = 5;
	posx = 320;
	posy = 240;
	//Setting sprite
	Rectangle.setFillColor(sf::Color(255, 255, 255, 150));
	Rectangle.setSize({ 10.f, 10.f });
}

int screen_1::Run(sf::RenderWindow &window)
{
	try
	{
		Load();
	}
	catch (const std::exception &) 
	{
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
}

int main(int argc, char** argv)
{
	std::vector<cScreen*> Screens;
	screen_0 s0;
	Screens.push_back(&s0);
	screen_1 s1;
	int screen = 0;
	RenderWindow Res(VideoMode(400, 400), "SFML works!");
	Screens.push_back(&s1);
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(Res);
	}

	return EXIT_SUCCESS;
}
	

