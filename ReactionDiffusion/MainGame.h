#pragma once
#include "Solver.h"
#include <memory>
#include <SFML\Window.hpp>
class MainGame
{
public:
	MainGame();
	~MainGame();

	void Update(sf::Window& window);
	void Draw();

	void initFrame(GLManager & glManager);

	void HandleEvent(sf::Window& window, sf::Event& e);

private:
	 std::unique_ptr<Solver> solver;
	 GLManager gl;
	 GLuint fbo;
	 GLuint textures[2];
	 const int width = 200, height = 200;
	 const int wwidth = 800, wheight = 800;


	 void saveRegion(const std::string& filename, glm::ivec2 first, glm::ivec2 last);

	 void loadRegion(
		 const std::string& filename,
		 glm::ivec2 position);


	 void findStructures();
};