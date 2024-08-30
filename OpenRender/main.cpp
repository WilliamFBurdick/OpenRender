#include <cstdio>
#include "Screen.h"
#include "Input.h"


int main(int argc, char* argv[])
{
	if (!Screen::Instance()->Init())
	{
		return -1;
	}
	//=========================================================================
	// MAIN LOOP
	//=========================================================================
	bool isRunning = true;
	while (isRunning)
	{
		Input::Instance()->Update();
		if (Input::Instance()->IsXClicked())
		{
			isRunning = false;
		}
		Screen::Instance()->Clear();

		Screen::Instance()->Present();
	}
	Screen::Instance()->Shutdown();
	return 0;
}