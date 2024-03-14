#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Texture.h"
namespace GE
{
	class SkyDome
	{
	public:
		SkyDome(const char* textPath, int resolution, int radius);
		void BindTexture(GLuint* PIDref);
		inline GLuint getVertices()
		{
			return vbo;
		}

		inline GLuint getIndices()
		{
			return ibo;
		}
		GLuint getIndexCount()
		{
			return indexCount;
		}
		~SkyDome()
		{
			delete texture;
			texture = nullptr;
		}

	public:
		GLuint vbo, ibo, indexCount;
		Texture* texture = nullptr;
	};
}

