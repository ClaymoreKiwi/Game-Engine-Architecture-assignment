#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

namespace GE 
{
	class Texture
	{
	public:
		Texture(std::string fileName)
		{
			loadTexture(fileName);
		}

		~Texture(){}

		const int getWidth()
		{
			return width;
		}

		const int getHeight()
		{
			return height;
		}

		const Uint32 getFormat()
		{
			return format;
		}

		const GLuint getTexture()
		{
			return textureName;
		}

	private:
		void loadTexture(std::string fileName);

	private:
		int		width = 0,
				height = 0;
 
		Uint32  format = 0;
 
		GLuint  textureName = 0;
	};
}

