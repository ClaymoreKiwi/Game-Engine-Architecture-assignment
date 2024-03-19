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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//create window - fixed window size
		app_Window.reset(SDL_CreateWindow("Straight6", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
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

		//variable setup
		fontRenderer = new GE::FontRendering();
		fontRenderer->BindTexture("hello world", "../models/fonts/Oswald-Regular.ttf", 24);

		mr = std::make_unique<GE::ModelRenderer>();

		skydome = std::make_unique<GE::SkyDome>("../models/SkyDome/skydomeDark.jpg", 16, 10);
		skyMr =   std::make_unique<GE::ModelRenderer>();

		terrain =   std::make_unique<GE::Terrain>("../models/terrain/island-texture.png","../models/terrain/island-map.png", 20.0f, 500.0f);
		terrainMr = std::make_unique<GE::ModelRenderer>();

		//init cam
		fpscam = std::make_unique<GE::FPSCamera>(glm::vec3(0.0f, 0.0f, 0.0f) + dist,//look at
									glm::vec3(250.0f, 50.0f, 500.0f),//pos
									glm::vec3(0.0f, 1.0f, 0.0f),//Up direction
									45.0f, 640.0f / 480.0f, .1f, 1000.0f);

		billboard = std::make_unique<GE::Billboard>("../models/tree.png", 250, 0, 250);

		billboard->setScaleX(10.0f);
		billboard->setScaleY(10.0f);

		billboardRenderer = std::make_unique<GE::BillboardRenderer>();

		LoadModels();
		//success - return true
		return true;
	}

	void GameEngine::LoadModels()
	{
		for (int i = 0; i < size; i++)
		{
			if (listOfModels[i] != "../models/Trees.obj")
			{
				modelsToLoad.push_back(std::make_unique<GE::Model>(mr->getPID(), listOfModels[i], listOfTextures[i]));
				continue;
			}
			modelsToInstance.push_back(std::make_unique<GE::Model>(mr->getPID(), listOfModels[i], listOfTextures[i]));
		}
		mr->setPos(260.0f, 0.0f, 300.0f);
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

	void GameEngine::Draw()
	{
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyMr->Draw(fpscam.get(), skydome.get());
		terrainMr->Draw(fpscam.get(), terrain.get());
		for (auto& model : modelsToLoad)
		{
			mr->Draw(fpscam.get(), model.get());
		}
		for (auto& model : modelsToInstance)
		{
			mr->Draw(fpscam.get(), model.get());
		}
		billboardRenderer->draw(billboard.get(), fpscam.get());
		fontRenderer->RenderText();
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

