#pragma once
#include "GLManager.h"
#include <vector>
#include <glm\glm.hpp>
#include <memory>
#include "Framebuffer.h"

class Solver
{
public:
	Solver(GLManager& gl);
	~Solver();

	void Update();
	void Draw(GLManager& manager);

private:
	GLuint vbo, vao, ibo;

	void generateBuffers(GLuint program);
	void generateVertices();
	void updateBuffers();
	std::vector<glm::vec3> vertices;
	std::vector<unsigned> indices;

	std::unique_ptr<Framebuffer> framebuffer;
};

