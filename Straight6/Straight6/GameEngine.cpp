#include "GameEngine.h"
#include <iostream>

namespace GC {

	bool GC::GameEngine::Init(bool enableVsync)
	{
		time = new deltaTime();
		//initialising SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "unable to initialise SDL! Error: " << SDL_GetError() << std::endl;
			return false;
		}

		//set OpenGL version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
		//set the profile to core - modern OpenGL
		//no legacy features for backwards compatibility
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//create window - fixed window size
		app_Window = SDL_CreateWindow("Straight6", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (app_Window == nullptr)
		{
			std::cerr << "unable to create window! Error: " << SDL_GetError() << std::endl;
			return false;
		}

		//Create Gl Context using SDL
		app_GlContext = SDL_GL_CreateContext(app_Window);
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
		fontRenderer = new GE::FontRendering(app_Window, width, height);

		mr = new GE::ModelRenderer();
		//skyboxRenderer = new GE::SkyBox(skySides);

		skydome = new GE::SkyDome("../models/SkyDome/skydomeDark.jpg", 16, 10);
		skyMr = new GE::ModelRenderer();

		terrain = new GE::Terrain("../models/terrain/island-texture.png","../models/terrain/island-map.png", 20.0f, 500.0f);
		terrainMr = new GE::ModelRenderer();

		//init cam
		fpscam = new GE::FPSCamera(glm::vec3(0.0f, 0.0f, 0.0f) + dist,//look at
									glm::vec3(250.0f, 50.0f, 500.0f),//pos
									glm::vec3(0.0f, 1.0f, 0.0f),//Up direction
									45.0f, 640.0f / 480.0f, .1f, 1000.0f);

		billboard = new GE::Billboard("../models/tree.png");

		billboard->setScaleX(10.0f);
		billboard->setScaleY(10.0f);

		billboard->setZ(-10.0f);

		billboardRenderer = new GE::BillboardRenderer();

		billboardRenderer->init();

		LoadModels();
		mr->setPos(260.0f, 0.0f, 300.0f);
		//success - return true
		return true;
	}

	void GameEngine::LoadModels()
	{
		for (int i = 0; i < size; i++)
		{
			modelsToLoad.push_back(new GE::Model(mr->getPID(), listOfModels[i], listOfTextures[i]));
		}
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

		SDL_WarpMouseInWindow(app_Window, width / 2, height / 2);

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
		int colour[4] ={255,255,255,255};
		fontRenderer->draw("FPS: ", 100, 50, 150, 50, colour);
		mr->setRotation(0.0f, -180.0f, 0.0f);
	}

	void GameEngine::Draw()
	{
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//skyboxRenderer->Draw(camera);
		skyMr->Draw(fpscam, skydome);
		terrainMr->Draw(fpscam, terrain);
		for (auto& model : modelsToLoad)
		{
			mr->Draw(fpscam, model);
		}
		billboardRenderer->draw(billboard, fpscam);
		SDL_GL_SwapWindow(app_Window);
	}

	void GameEngine::ShutDown()
	{
		delete mr;
		mr = nullptr;
		delete terrain;
		terrain = nullptr;
		delete terrainMr;
		terrainMr = nullptr;

		for (auto& model : modelsToLoad)
		{
			delete model;
			model = nullptr;
		}
		modelsToLoad.clear();
		delete fpscam;

		SDL_DestroyWindow(app_Window);

		app_Window = nullptr;

		SDL_Quit();
	}

	void GameEngine::SetWindowTile()
	{
		if (time->GetDeltaTime() > 1000)
		{
			std::ostringstream msg;
			msg << "FPS = " << FPSCounter;
			FPSCounter = 0;
			SDL_SetWindowTitle(app_Window, msg.str().c_str());
			time->ManualReset();
		}
	}
}

