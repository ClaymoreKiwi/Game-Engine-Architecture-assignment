#include "FrameBuffer.h"
#include <iostream>

bool GE::FrameBuffer::init()
{
	GLCall(glGenBuffers(1, &fboScene));
	GLCall(glBindBuffer(GL_FRAMEBUFFER, fboScene));

	GLuint sceneTextureName;

	// Create a texture for the framebuffer.
	glGenTextures(1, &sceneTextureName);
	glBindTexture(GL_TEXTURE_2D, sceneTextureName);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,sceneTextureName,0);

	glGenRenderbuffers(1, &depthBufferName);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferName);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);

	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
			
	glDrawBuffers(1, drawBuffers);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "FB error, status" << status << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	sceneTexture = new Texture(sceneTextureName);

	return true;
}

void GE::FrameBuffer::beginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboScene);
}

void GE::FrameBuffer::endRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
