#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <memory>
#include "Texture.h"

namespace GE
{
	class Terrain
	{
	public:

		Terrain(const char* texturePath, const char* heightMapPath, float HightScale, float Scale);
		void BindTexture(const GLuint* PIDref);
		~Terrain(){}
		const inline GLuint getVertices()
		{
			return vbo;
		}
		
		const inline GLuint getIndices()
		{
			return ibo;
		}
		const inline GLuint getIndexCount()
		{
			return indexCount;
		}
	private:
		GLuint vbo, ibo, indexCount;
		std::unique_ptr<GE::Texture> texture = nullptr;
	};
}

