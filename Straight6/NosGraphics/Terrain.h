#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Texture.h"

namespace GE
{
	class Terrain
	{
	public:

		Terrain(const char* texturePath, const char* heightMapPath, float HightScale, float Scale);
		void BindTexture(const GLuint* PIDref);
		~Terrain()
		{
			delete texture;
		}
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
	private:
		GLuint vbo, ibo, indexCount;
		GE::Texture* texture = nullptr;
	};
}

