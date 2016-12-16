#include "Framebuffer.h"



Framebuffer::Framebuffer(int width, int height, int textureAttachment)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textureAttachment, GL_TEXTURE_2D, texColorBuffer, 0
	);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}


Framebuffer::~Framebuffer()
{
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


