#include <iostream>

#include "PitWizard.h"

// Main entry point for the application
int main()
{
	auto app = std::unique_ptr<GG::Application>( new PitWizard() );

	app->init();
	app->doGameLoop();
	app->shutdown();

	// Return
	return 0;
}
