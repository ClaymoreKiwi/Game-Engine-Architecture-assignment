#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "DeltaTime.h"
#include "FPSCamera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "SkyDome.h"
#include "Terrain.h"
#include "Billboard.h"
#include "BillboardRenderer.h"
#include "FontRendering.h"

namespace GC
{
	class GameEngine
	{

	public:
		GameEngine()
			:app_Window(nullptr, SDL_DestroyWindow){}	//constructor
		virtual ~GameEngine() 
		{
			ShutDown();
		}	//destructor

		bool Init(bool enableVSync);  //initialisation
		void LoadModels();			  //load all models
		bool KeepRunning();			  //indication of the scene state
		void processInput();		  //input processing
		void Update();				  //engine update logic func
		void Draw();				  //rendering updates
		void ShutDown();			  //closing the engine down / cleaning the application on close

		//extra methods
		void SetWindowTile();

		bool fullScreen = false;
		int width = 640, height = 480;
	private:
		SDL_GLContext      app_GlContext = NULL;	//represents OpenGL in the engine
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> app_Window;    //represent the engine window
		std::unique_ptr<deltaTime>		   time = nullptr;			//Deta time object for frame counting and timing processes against
		
		glm::vec3 dist = {};
		
		std::unique_ptr<GE::Terrain>			terrain = nullptr;
		std::unique_ptr<GE::ModelRenderer>		terrainMr = nullptr;

		std::unique_ptr<GE::FPSCamera>			fpscam = nullptr;		//perspective cam
		std::unique_ptr<GE::ModelRenderer>		mr = nullptr;			//redering class for the model to be passed into

		std::unique_ptr<GE::SkyDome>			skydome = nullptr;
		std::unique_ptr<GE::ModelRenderer>		skyMr = nullptr;

		std::unique_ptr<GE::Billboard> billboard = nullptr;
		std::unique_ptr<GE::BillboardRenderer> billboardRenderer = nullptr;

		GE::FontRendering* fontRenderer = nullptr;

		short int FPSCounter = 0;
		static const int size = 5;

		///////////////////////////////////////////////////////////////////////////
		//arrays
		//////////////////////////////////////////////////////////////////////////
		//list of models i will be loading in the scene
		std::vector< std::unique_ptr<GE::Model>> modelsToLoad;
		std::vector< std::unique_ptr<GE::Model>> modelsToInstance;
		const char* listOfModels[size] =
		{
			"../models/tenticle1.obj",
			"../models/boat.obj",
			"../models/cannon.obj",
			"../models/Trees.obj",
			"../models/crystal.obj"
		};
		const char* listOfTextures[size] =
		{
			"../models/Tenticle_colour.jpg",
			"../models/BoatUV.jpg",
			"../models/cannonUV.jpg",
			"../models/treeUV.jpg",
			"../models/crystal.jpg"
		};
	};

	//helper functions
	inline void Display_Info_Message(const char* message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", message, nullptr);
	}
}

