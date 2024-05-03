#pragma once
#include <glew.h>
#include <SDL_opengl.h>
#include "Texture.h"

namespace GE {
	class FrameBuffer
	{
	public:
		FrameBuffer(int w, int h) {
			width = w;
			height = h;

			fboScene = -1;
			sceneTexture = nullptr;
			depthBufferName = -1;
		}

		~FrameBuffer() {
			// Release the texture, depth buffer and framebuffer objects
			delete sceneTexture;
			glDeleteRenderbuffers(1, &depthBufferName);
			glDeleteFramebuffers(1, &fboScene);
		}

		bool init();
		void beginRender();
		void endRender();

		// Provides access to the texture object
		// associated with the framebuffer
		Texture* getTexture() {
			return sceneTexture;
		}

	private:
		GLuint fboScene;
		GLuint depthBufferName;
		Texture* sceneTexture = nullptr;

		int width, height;
	};

}

