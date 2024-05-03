#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "DeltaTime.h"
#include "FPSCamera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "InstancedRenderer.h"
#include "SkyDome.h"
#include "Terrain.h"
#include "ParticleEmitter.h"
#include "Light.h"

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
		void ModelDraw();			  //draws models
		void ShadowPass();			  //does shadow pass
		void Draw();				  //rendering updates
		void ShutDown();			  //closing the engine down / cleaning the application on close

		//extra methods
		void SetWindowTile();

		bool fullScreen = false;
		int width = 1080, height = 720;
	private:

		SDL_GLContext      app_GlContext = NULL;	//represents OpenGL in the engine
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> app_Window;    //represent the engine window
		std::unique_ptr<deltaTime>		   time = nullptr;			//Deta time object for frame counting and timing processes against
		
		std::unique_ptr<GE::Terrain>			terrain = nullptr;
		std::unique_ptr<GE::ModelRenderer>		terrainMr = nullptr;

		std::unique_ptr<GE::FPSCamera>			fpscam = nullptr;		//perspective cam
		std::unique_ptr<GE::ModelRenderer>		mr = nullptr;			//redering class for the model to be passed into
		std::unique_ptr<GE::InstancedRenderer>  ir = nullptr;

		std::unique_ptr<GE::SkyDome>			skydome = nullptr;
		std::unique_ptr<GE::ModelRenderer>		skyMr = nullptr;

		std::unique_ptr<GE::ParticleEmitter>    emitter = nullptr;
		std::unique_ptr<GE::Light> light = nullptr;

		short int FPSCounter = 0;
		static const int size = 6;

		///////////////////////////////////////////////////////////////////////////
		//arrays
		//////////////////////////////////////////////////////////////////////////
		//list of models i will be loading in the scene
		struct ModelData
		{
			const char* modelPath;
			const char* texturePath;
			const char* billboardPath;
			glm::vec3 location;
		};

		ModelData listOfModels[size] =
		{
			{"../models/tenticle1.obj", "../models/Tenticle_colour.jpg", "../models/billboards/Tenticle-BB.png", glm::vec3(275.0f, -3.0f, 300.0f)},
			{"../models/boat.obj"	  , "../models/BoatUV.jpg"		   , "../models/billboards/Boat-BB.png"	   , glm::vec3(260.0f, 3.0f, 300.0f)},
			{"../models/cannon.obj"	  , "../models/cannonUV.jpg"	   , "../models/billboards/cannon-BB.png"  , glm::vec3(257.0f, 7.0f, 295.0f)},
			{"../models/crystal.obj"  , "../models/crystal.jpg"		   , "../models/billboards/crystal-BB.png" , glm::vec3(232.0f, 4.2f, 301.0f)},
			{"../models/Chest.obj"	  , "../models/chest.png"		   , "../models/billboards/chest-BB.png"   , glm::vec3(230.0f, 4.5f, 300.0f)},
			{"../models/volcano.obj"  , "../models/volcano.png"		   , "../models/billboards/replace.png"    , glm::vec3(100.0f, 0.0f, 100.0f)}
		};
		ModelData listOfInstanceModels[size] =
		{
			{"../models/Trees.obj"	  , "../models/treeUV.jpg"		   , "../models/billboards/Tree-BB.png"	   , glm::vec3(230.0f, 2.0f, 300.0f)},
		};
		std::vector< std::unique_ptr<GE::Model>> modelsToLoad;
		std::vector< std::unique_ptr<GE::Model>> modelsToInstance;
	};

	//helper functions
	inline void Display_Info_Message(const char* message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", message, nullptr);
	}
}

