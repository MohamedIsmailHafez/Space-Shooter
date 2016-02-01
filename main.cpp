#include "GameConfig.h"
#include "Renderer.h"
#include "GameManager.h"
#include "PlayerShip.h"

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	GameConfig& config = GameConfig::getInstance();

	config.ScreenWidth  = 1280;
	config.ScreenHeight = 720;

	Renderer::getInstance().InitializeRenderer(argc, argv);

	GameManager::getInstance().Initialize(new PlayerShip);

	glutMainLoop();
	
}
