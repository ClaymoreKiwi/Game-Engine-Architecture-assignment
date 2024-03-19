#pragma once
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <iostream>

#include "Billboard.h"
#include "BillboardRenderer.h"
#include "Texture.h"

namespace GE
{
	class FontRendering
	{
	public:
		FontRendering()
		{
			init();
		};
		void init();
		void BindTexture(std::string text, std::string fontName, int size);
		void RenderText();
		void clean();
		
		~FontRendering()
		{
			clean();
		}
	private:
		Texture* textureText = nullptr;

		Billboard* billboard = nullptr;
		BillboardRenderer* billboardRenderer = nullptr;
	};
}



