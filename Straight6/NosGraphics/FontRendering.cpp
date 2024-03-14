#include "FontRendering.h"

GE::FontRendering::FontRendering(SDL_Window* window, const int& screenW, const int& screenH)
	: app_window(window), screenWidth(screenW), screenHeight(screenH)
{
	init();
}

void GE::FontRendering::init()
{
	renderer = SDL_CreateRenderer(app_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//if renderer could not be initialised throw corresponding error
	if (renderer == NULL)
	{
		std::cerr << "could not initialize renderer!\n";
		std::cerr << SDL_GetError() << std::endl;
	}
	int init = TTF_Init();
	if (init != 0)
	{
		std::cerr << SDL_GetError() << std::endl;
	}
	//this constant text is for simplicity - at no point will it change font set
	//so as a reasonable point to keep this as is
	font = TTF_OpenFont("../extern/content/font/OpenSans-Regular.ttf", 100);
	if (font == NULL)
	{
		std::cerr << SDL_GetError() << std::endl;
	}
}

void GE::FontRendering::draw(std::string text)
{
	//default 
	SDL_Color colour = SDL_Color();
	colour.r = 0; //red
	colour.g = 0; //green
	colour.b = 0; //blue
	colour.a = 255; // alpha 

	//create a surface for the font
	SDL_Surface* textImg = TTF_RenderText_Solid(font, text.c_str(), colour);
	//create a texture to display from the sureace
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textImg);
	//render copy
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	//free resources
	SDL_FreeSurface(textImg);
	SDL_DestroyTexture(texture);
}

void GE::FontRendering::draw(std::string text, const int& posX, const int& posY, int&& width, int&& height, int rgba[4])
{
	SDL_Color colour = SDL_Color();
	colour.r = rgba[0]; //red
	colour.g = rgba[1]; //green
	colour.b = rgba[2]; //blue
	colour.a = rgba[3]; //alpha 

	//create a surface for the font
	SDL_Surface* textImg = TTF_RenderText_Solid(font, text.c_str(), colour);
	//create a texture to display from the sureace
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textImg);
	SDL_Rect fontSize;
	fontSize.w = width;
	fontSize.h = height;
	int middleX = ((0 + fontSize.w) / 2);
	int middleY = ((0 + fontSize.h) / 2);
	fontSize.x = posX - middleX;
	fontSize.y = posY - middleY;

	SDL_RenderClear(renderer);
	//render copy
	SDL_RenderCopy(renderer, texture, NULL, &fontSize);
	SDL_RenderPresent(renderer);
	//free resources
	SDL_FreeSurface(textImg);
	SDL_DestroyTexture(texture);
}

void GE::FontRendering::clean()
{
	TTF_CloseFont(font);
	TTF_Quit();
}
