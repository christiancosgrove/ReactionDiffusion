#pragma once
#include <GL\glew.h>
class Framebuffer
{
public:
	Framebuffer(int width, int height, int textureAttachment);
	~Framebuffer();
	void Bind();
	void Unbind();
private:
	GLuint fbo;
};

