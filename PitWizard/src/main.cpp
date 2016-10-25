#include <iostream>

#include "PitWizard.h"

// Main entry point for the application
int main(int argc, char** argv)
{
	auto app = std::unique_ptr<GG::IApplication>( new PitWizard() );

	app->init(argc, argv);
	app->doGameLoop();
	app->shutdown();

	// Return
	return 0;
}
