#include <stdlib.h>

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#include "..\Asteroid.h"

enum class MainState
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

j1App* App = NULL;

int main(int argc, char* args[])
{
	MainState state = MainState::CREATE;
	int result = EXIT_FAILURE;

	ASTEROID_INIT();

	while(state != MainState::EXIT)
	{
		switch(state)
		{

			// Allocate the engine --------------------------------------------
			case MainState::CREATE:
			LOG("CREATION PHASE ===============================");

			App = new j1App(argc, args);

			if(App != NULL)
				state = MainState::AWAKE;
			else
				state = MainState::FAIL;

			break;

			// Awake all modules -----------------------------------------------
			case MainState::AWAKE:
			LOG("AWAKE PHASE ===============================");
			if(App->Awake() == true)
				state = MainState::START;
			else
			{
				LOG("ERROR: Awake failed");
				state = MainState::FAIL;
			}

			break;

			// Call all modules before first frame  ----------------------------
			case MainState::START:
			LOG("START PHASE ===============================");
			if(App->Start() == true)
			{
				state = MainState::LOOP;
				LOG("UPDATE PHASE ===============================");
			}
			else
			{
				state = MainState::FAIL;
				LOG("ERROR: Start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
			case MainState::LOOP:
			{
				ASTEROID_FRAME();
				if (App->Update() == false)
					state = MainState::CLEAN;
			} break;

			// Cleanup allocated memory -----------------------------------------
			case MainState::CLEAN:
			LOG("CLEANUP PHASE ===============================");
			if(App->CleanUp() == true)
			{
				RELEASE(App);
				result = EXIT_SUCCESS;
				state = MainState::EXIT;
			}
			else
				state = MainState::FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
			case MainState::FAIL:
			LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = MainState::EXIT;
			break;
		}
	}

	ASTEROID_CLEANUP();

	return result;
}