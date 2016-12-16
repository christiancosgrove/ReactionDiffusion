#include "Solver.h"

using namespace glm;

Solver::Solver(GLManager& gl)
{
	generateBuffers(gl.Programs[0].programID);
	generateVertices();
	updateBuffers();

	//framebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(800, 600, 0));
}


Solver::~Solver()
{
}

void Solver::Update()
{

}

void Solver::Draw(GLManager& manager)
{
	//framebuffer->Bind();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	//framebuffer->Unbind();
}

void Solver::generateBuffers(GLuint program)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	GLint posAttrib = glGetAttribLocation(program, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glEnableVertexAttribArray(posAttrib);

	glBindVertexArray(0);
}

void Solver::generateVertices()
{
	vertices.push_back(vec3(-1, -1, 0));
	vertices.push_back(vec3(1, -1, 0));
	vertices.push_back(vec3(-1, 1, 0));
	vertices.push_back(vec3(1, 1, 0));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
}

void Solver::updateBuffers()
{
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), &vertices[0], GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), &indices[0], GL_STREAM_DRAW);
	glBindVertexArray(0);

}
