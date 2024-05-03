#include "GameEngine.h"

int main(int argc, char* argv[])
{
	//game engine object
	GC::GameEngine* ge = new GC::GameEngine;

	//initialised object
	if (!ge->Init(true))
	{
		GC::Display_Info_Message("Couldn't instantiate SDL. Check console for more details");
		return -1;
	}

	while (ge->KeepRunning())
	{
		ge->Update();
		ge->Draw();
	}
	ge->ShutDown();
	delete ge;
	ge = nullptr;
	return 0;
}