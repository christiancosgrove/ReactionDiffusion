#include <iostream>
#include <SFML\Window.hpp>
#include <gl\glew.h>
#include<SFML\OpenGL.hpp>
#include "MainGame.h"

int main() {
	sf::Window window(sf::VideoMode(800,800), "OpenGL", sf::Style::Default);
	glewInit();
	MainGame game;

	window.setKeyRepeatEnabled(false);


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			default:
				game.HandleEvent(window, event);
			}
		}
		window.setActive();

		game.Update(window);
		game.Draw();

		window.display();
	}
}