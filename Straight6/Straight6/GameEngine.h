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

namespace GC
{
	class GameEngine
	{

	public:
		GameEngine(){}				//constructor
		virtual ~GameEngine() {}	//destructor

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
		SDL_Window*		   app_Window = nullptr;    //represent the engine window
		SDL_GLContext      app_GlContext = NULL;	//represents OpenGL in the engine
		deltaTime*		   time = nullptr;			//Deta time object for frame counting and timing processes against
		
		glm::vec3 dist = {};
		
		GE::Terrain*			terrain = nullptr;
		GE::ModelRenderer*		terrainMr = nullptr;

		GE::FPSCamera*			fpscam = nullptr;		//perspective cam
		GE::ModelRenderer*		mr = nullptr;			//redering class for the model to be passed into
		//GE::SkyBox*				skyboxRenderer = nullptr;

		GE::SkyDome*			skydome = nullptr;
		GE::ModelRenderer*		skyMr = nullptr;

		GE::Billboard* bb = nullptr;
		GE::BillboardRenderer* bbr = nullptr;

		short int FPSCounter = 0;
		static const int size = 5;

		///////////////////////////////////////////////////////////////////////////
		//arrays
		//////////////////////////////////////////////////////////////////////////
		//list of models i will be loading in the scene
		std::vector<GE::Model*> modelsToLoad;
		const char* listOfModels[size] =
		{
			"../models/boat.obj",
			"../models/cannon.obj",
			"../models/tenticle1.obj",
			"../models/Trees.obj",
			"../models/crystal.obj"
		};
		const char* listOfTextures[size] =
		{
			"../models/boatUV.jpg",
			"../models/cannonUV.jpg",
			"../models/Tenticle_colour.jpg",
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

