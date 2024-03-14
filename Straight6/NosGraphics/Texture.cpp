#include "Texture.h"
#include <iostream>

void GE::Texture::loadTexture(std::string fileName)
{
	//load texture on surface
	SDL_Surface* surfaceImage = IMG_Load(fileName.c_str());
	//check if not null
	if (surfaceImage == nullptr)
		return;
	//assign variables to surface paraneters
	width = surfaceImage->w;
	height = surfaceImage->h;
	format = surfaceImage->format->format;

	//getermine what OpenGL format to set
	switch (format)
	{
	case SDL_PIXELFORMAT_RGBA32:
		format = GL_RGBA;
		break;
	case SDL_PIXELFORMAT_RGB24:
		format = GL_RGB;
		break;
	default:
		format = GL_RGB;
		break;
	}
	//create texture name from the assigned texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);
	//subsequent texture setup for openGL
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	//configure the weighting and what to manuipulate when texture is mapped on a model
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//release surface
	SDL_FreeSurface(surfaceImage);

	return;
}
