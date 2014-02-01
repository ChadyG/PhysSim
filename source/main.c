/*
  Main module, will manage the general hierarchy of 
  the game.

  Also, there is one of these little documenting snippets
  in each of the header files to provide information on the module's
  use, and whatever other notes you think need to be written down.
*/
#include <PA9.h>       // Include for PA_Lib
#include "data.h"
#include "editor.h"

int main(int argc, char ** argv) {
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_SplashEyes();  // give props to Mollusk
  //make our own splash at some point

  //for now, we'll just go into the editor 
  //(until someone starts making a menu that is)
  editor();
	
	return 0;
} // End of main()
