#include <GLFW/glfw3.h>

import baya;

// Define the entry point based on the build type
#ifdef ENTRY_POINT
#define MAIN_ENTRY ENTRY_POINT
#else
#define MAIN_ENTRY main
#endif

int ENTRY_POINT(int argc, char** argv)
{
	init_game();
	main_loop();
	cleanup();
}