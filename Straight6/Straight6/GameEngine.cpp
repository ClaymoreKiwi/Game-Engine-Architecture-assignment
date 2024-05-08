#include "GameEngine.h"
#include <iostream>
namespace GC {

	bool GC::GameEngine::Init(bool enableVsync)
	{
		time = std::make_unique<deltaTime>();
		//initialising SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "unable to initialise SDL! Error: " << SDL_GetError() << std::endl;
			return false;
		}

		//set OpenGL version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		//create window - fixed window size
		app_Window.reset(SDL_CreateWindow("Straight6", 50, 50, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
		if (app_Window == nullptr)
		{
			std::cerr << "unable to create window! Error: " << SDL_GetError() << std::endl;
			return false;
		}

		//Create Gl Context using SDL
		app_GlContext = SDL_GL_CreateContext(app_Window.get());
		if (app_GlContext == nullptr)
		{
			std::cerr << "SDL could not create GL context! Error: " << SDL_GetError() << std::endl;
			return false;
		}

		//initialsing GLEW
		//binds latest extensions rather than defult OpenGL 1.1
		GLenum status = glewInit();
		if (status != GLEW_OK)
		{
			std::cerr << "Error initialising GLEW! Error: " << glewGetErrorString(status) << std::endl;
			return false;
		}

		if (enableVsync == true)
		{
			std::cout << "VSync Enabled\n";
			//temp vsync attribute
			if (SDL_GL_SetSwapInterval(1) != 0)
			{
				std::cerr << "Warning: Unable to set VSync! Error: " << SDL_GetError() << std::endl;
				return false;
			}
		}
		else std::cout << "VSync Disabled\n";


		skydome = std::make_unique<GE::SkyDome>("../models/SkyDome/skydomeDark.png", 16, 10);
		skyMr =   std::make_unique<GE::ModelRenderer>();

		terrain =   std::make_unique<GE::Terrain>("../models/terrain/island-texture.png","../models/terrain/island-map.png", 20.0f, 500.0f);
		terrainMr = std::make_unique<GE::ModelRenderer>();

		//init cam
		fpscam = std::make_unique<GE::FPSCamera>(glm::vec3(0.0f, 0.0f, 0.0f),//look at
									glm::vec3(250.0f, 30.0f, 400.0f),//pos
									glm::vec3(0.0f, 1.0f, 0.0f),//Up direction
									45.0f, 640.0f / 480.0f, .1f, 1000.0f);
		
		light = std::make_unique<GE::Light>(glm::vec3(250.0f, 0.0f, 300.0f), glm::vec3(300.0f, 250.0f, 0.0f), fpscam.get());
		glm::vec3 colour = {1,1,1}, ambient = {0.3,0.3,0.3};
		light->SetLight(colour, ambient);
		light->MakeShadowMap();
		
		mr = std::make_unique<GE::ModelRenderer>();
		ir = std::make_unique<GE::InstancedRenderer>();
		ir->createInstances(20);
		
		LoadModels();
		emitter = std::make_unique<GE::ParticleEmitter>(20, 10.0f,glm::vec3(100.0f, 90.0f, 100.0f), "../models/billboards/smoke.png");

		return true;
	}

	void GameEngine::LoadModels()
	{
		for (int i = 0; i < size; i++)
		{
			modelsToLoad.push_back(std::make_unique<GE::Model>(mr->getPID(), listOfModels[i].modelPath, listOfModels[i].texturePath, listOfModels[i].billboardPath, listOfModels[i].location));
		}
		modelsToInstance.push_back(std::make_unique<GE::Model>(mr->getPID(), listOfInstanceModels[0].modelPath, listOfInstanceModels[0].texturePath, listOfInstanceModels[0].billboardPath, listOfInstanceModels[0].location));
	}

	bool GameEngine::KeepRunning()
	{
		SDL_PumpEvents();

		SDL_Event event;
		if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT))
		{
			//quitting code can come here
			return false;
		}
		return true;
	}

	void GameEngine::processInput()
	{
		//move for later setting controls
		float camSpeed = 0.5f;
		const float mouseSens = 0.1f;

		int mouseX, MouseY;
		SDL_GetMouseState(&mouseX, &MouseY);

		int diffX = mouseX - (width / 2);
		int diffY = (height / 2) - MouseY;

		fpscam->setYaw(fpscam->getYaw() + diffX * mouseSens);
		fpscam->setPitch(fpscam->getPitch() + diffY * mouseSens);

		SDL_WarpMouseInWindow(app_Window.get(), width / 2, height / 2);
		SDL_ShowCursor(SDL_DISABLE);

		glm::vec3 direction;
		direction.x = cos(glm::radians(fpscam->getYaw())) * cos(glm::radians(fpscam->getPitch()));
		direction.y = sin(glm::radians(fpscam->getPitch()));
		direction.z = sin(glm::radians(fpscam->getYaw())) * cos(glm::radians(fpscam->getPitch()));
		fpscam->setTarget(glm::normalize(direction));

		bool keyStates[5] = { 0,0,0,0,0};
		enum {
			UP = 0,
			DOWN,
			LEFT,
			RIGHT,
			SHIFT
		};

		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			//SDL built in function checks every key state all the time
			//its able to then set the flag appropriately for the update (and skips the key repeat delay all together)
			const Uint8* state = SDL_GetKeyboardState(NULL);
			//use this as the flag to check the state of key presses
			keyStates[UP] = state[SDL_SCANCODE_W] > 0;
			keyStates[DOWN] = state[SDL_SCANCODE_S] > 0;
			keyStates[LEFT] = state[SDL_SCANCODE_A] > 0;
			keyStates[RIGHT] = state[SDL_SCANCODE_D] > 0;
			keyStates[SHIFT] = state[SDL_SCANCODE_LSHIFT] > 0;
		}

		if (keyStates[SHIFT])
			camSpeed += camSpeed;
		if (keyStates[UP])
			fpscam->setPos(fpscam->getPos() + fpscam->getTarget() * camSpeed);
		if (keyStates[DOWN])
			fpscam->setPos(fpscam->getPos() - fpscam->getTarget() * camSpeed);
		if (keyStates[LEFT])
			fpscam->setPos(fpscam->getPos() - glm::normalize(glm::cross(fpscam->getTarget(), fpscam->getUpDir())) * camSpeed);
		if (keyStates[RIGHT])
			fpscam->setPos(fpscam->getPos() + glm::normalize(glm::cross(fpscam->getTarget(), fpscam->getUpDir())) * camSpeed);
		
		fpscam->updateCamMatrices();
	}

	void GameEngine::Update()
	{
		////////////////////////////
		time->PartialUpdate();
		FPSCounter++;
		SetWindowTile();
		////////////////////////////

		processInput();
		mr->setRotation(0.0f, -180.0f, 0.0f);
	}

	void GameEngine::ModelDraw()
	{
		for (auto& model : modelsToLoad)
		{
			mr->Draw(model.get(), light.get());
		}
	}

	void GameEngine::ShadowPass()
	{
		glEnable(GL_DEPTH);
		glViewport(0, 0, light->GetShadowWidth(), light->GetShadowHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, light->GetFBO());
		glClear(GL_DEPTH_BUFFER_BIT);
		ModelDraw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}

	void GameEngine::Draw()
	{
		ShadowPass();
		glClearColor(0.2705f, 0.2745f, 0.2823f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyMr->Draw(fpscam.get(), skydome.get(), light.get());
		terrainMr->Draw(fpscam.get(), terrain.get(), light.get());
		for (auto& model : modelsToLoad)
		{
			mr->Draw(fpscam.get(), model.get(), light.get());
		}
		emitter->Update(time->GetDeltaTime(), fpscam.get());
		for (auto& model : modelsToInstance)
		{
			ir->DrawInstanced(fpscam.get(), model.get(), light.get());
		}

		SDL_GL_SwapWindow(app_Window.get());
	}

	void GameEngine::ShutDown()
	{
		modelsToLoad.clear();
		SDL_Quit();
	}

	void GameEngine::SetWindowTile()
	{
		if (time->GetDeltaTime() > 1000)
		{
			std::ostringstream msg;
			msg << "FPS = " << FPSCounter;
			FPSCounter = 0;
			SDL_SetWindowTitle(app_Window.get(), msg.str().c_str());
			time->ManualReset();
		}
	}
}

