#include "FontRendering.h"

void GE::FontRendering::init()
{
	int init = TTF_Init();
	if (init != 0)
	{
		std::cerr << SDL_GetError() << std::endl;
	}
	billboardRenderer = new GE::BillboardRenderer();
}

void GE::FontRendering::BindTexture(std::string text, std::string fontName, int size)
{
	TTF_Font* font = TTF_OpenFont(fontName.c_str(), size);
	if (font == nullptr)
	{
		std::cerr << "error creating font: " << text << " " << TTF_GetError() << std::endl;
		//return nullptr;
	}
	SDL_Color white = { 255,255,255 };
	SDL_Surface* surfaceImage = TTF_RenderText_Solid(font, text.c_str(), white);

	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surfaceImage, SDL_PIXELFORMAT_RGBA32, 0);

	if (converted != nullptr)
	{
		SDL_FreeSurface(surfaceImage);
		surfaceImage = converted;
	}
	else
	{
		std::cerr << "error converting font: " << text << " " << SDL_GetError() << std::endl;
	}
	Uint32 format = GL_RGBA;

	if (SDL_MUSTLOCK(surfaceImage))
	{
		SDL_LockSurface(surfaceImage);
	}
	int bytesPerRow = surfaceImage->w * surfaceImage->format->BytesPerPixel;
	char* temp = new char[bytesPerRow];

	for (int y = 0; y < surfaceImage->h / 2; ++y)
	{
		memcpy(temp, ((char*)surfaceImage->pixels) + ((surfaceImage->h - y - 1) * bytesPerRow), bytesPerRow);

		memcpy(((char*)surfaceImage->pixels) + ((surfaceImage->h - y - 1) * bytesPerRow), (char*)surfaceImage->pixels + (y * bytesPerRow), bytesPerRow);

		memcpy((char*)surfaceImage->pixels + (y * bytesPerRow), temp, bytesPerRow);
	}
	delete[] temp;
	GLuint textureName;

	GLCall(glGenTextures(1, &textureName));

	GLCall(glBindTexture(GL_TEXTURE_2D, textureName));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, surfaceImage->w, surfaceImage->h, 0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	SDL_FreeSurface(surfaceImage);
	TTF_CloseFont(font);

	textureText = new Texture(textureName);
	billboard = new GE::Billboard(textureText);
	billboard->setScaleX(10.0f);
	billboard->setScaleY(10.0f);
}

void GE::FontRendering::RenderText()
{
	billboardRenderer->draw(billboard);
}

void GE::FontRendering::clean()
{
	TTF_Quit();
}
