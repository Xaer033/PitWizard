#include "PitWizard.h"

// Main entry point for the application
int main()
{
	GG::Application *	pitWizardApp = new PitWizard();

	pitWizardApp->init();
	pitWizardApp->doGameLoop();
	pitWizardApp->shutdown();

	delete( pitWizardApp );

	// Return
	return 0;
}
