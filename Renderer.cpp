#include "Renderer.h"
#include "GameConfig.h"
#include "GameManager.h"
#include "GameObject.h"
#include <glut.h>
#include <sstream>

int Renderer::mCurrentTime		= 0;
int Renderer::mPreviousTime		= 0;
int Renderer::mElapsedTime		= 0;
int Renderer::mLastFrameTime	= 0;
int Renderer::mPreviousFPSTime	= 0;
int Renderer::mFrameNumber		= 0;

char Renderer::mFps[50];
char Renderer::mHintText[200];

//=======================================================================
// Glut Configuration Function
//=======================================================================
void Renderer::InitializeRenderer(int argc, char** argv)
{
	glutInit(&argc, argv);  

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 

	glutInitWindowSize(GameConfig::getInstance().ScreenWidth, GameConfig::getInstance().ScreenHeight);  

	glutInitWindowPosition(100, 150); 

	glutCreateWindow("Space Shooter"); 

	//Register Rendering Callbacks
	glutDisplayFunc			(Display);
	glutReshapeFunc			(Reshape);

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	InitOpenGL();

	InitializeHintText();

	GameTextureManager::getInstance().LoadTextures();
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void Renderer::InitOpenGL(void)
{
	glClearColor(0.f, 0.f, 0.f, 0.f); 

	glPointSize(4);

	glMatrixMode(GL_PROJECTION); 

	glLoadIdentity(); 

	gluOrtho2D(0.0, GameConfig::getInstance().ScreenWidth, 0.0, GameConfig::getInstance().ScreenHeight);  

	glMatrixMode(GL_MODELVIEW); 

	glLoadIdentity(); 
}

//=======================================================================
// Reshape Function
//=======================================================================
void Renderer::Reshape(int fWidth, int fHeight)
{
	GameConfig& config = GameConfig::getInstance();

	config.ScreenWidth  = fWidth;
	config.ScreenHeight = fHeight;

	//Set Viewport Size and start position
	glViewport(0, 0, fWidth, fHeight);	

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();   

	gluOrtho2D(0.0, fWidth, 0.0, fHeight); 

	glMatrixMode(GL_MODELVIEW); 

	glLoadIdentity(); 
}

//=======================================================================
// Render List of Game Objects 
//=======================================================================
void Renderer::RenderGameObjects()
{
	auto gameObjects = GameManager::getInstance().GameObjects;

	for(GameObject* object : gameObjects)
	{
		object->GLRender();
	}
}

//=======================================================================
// FPS Function
//=======================================================================
void Renderer::CalculateFPS()
{
	// Code to compute frames per second
	mFrameNumber++;
	glColor3f(0.2, 0.2, 0.2);
	if (mCurrentTime - mPreviousFPSTime > 1000) {
		sprintf(mFps,"FPS %1.0f", mFrameNumber*1000.0/(mCurrentTime-mPreviousFPSTime));
		mPreviousFPSTime = mCurrentTime;
		mFrameNumber = 0;
	}
}

//=======================================================================
// Render Text
//=======================================================================
void Renderer::RenderText(const char* string, int x, int y, int scale) 
{ 
	const char *c; 

	glPushMatrix();

	glLoadIdentity();
	glTranslatef(x, y, 0); 
	glScalef((float)scale/10, (float)scale/10, 1); 

	for (c = string; *c != '\0'; c++) 
	{ 
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN , *c); 
	}  
	glPopMatrix();
}

//=======================================================================
// Display FPS Function
//=======================================================================
void Renderer::DisplayFPS()
{
	RenderText(mFps, 5, glutGet(GLUT_WINDOW_HEIGHT) - 20, 1);
}

//=======================================================================
// Display Score Function
//=======================================================================
void Renderer::DisplayScore()
{
	std::stringstream strs;
	strs << "Score: " << GameManager::getInstance().GetScore();
	std::string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();
	RenderText(char_type, glutGet(GLUT_WINDOW_WIDTH) - 220, glutGet(GLUT_WINDOW_HEIGHT) - 50, 2);
}

//=======================================================================
// Render Hint Text
//=======================================================================
void Renderer::DisplayHints()
{
	glColor3f(0.2f, 0.2f, 0.2f);

	RenderText(mHintText, glutGet(GLUT_WINDOW_WIDTH)/2 - 200, 30, 1);
}

//=======================================================================
// Render GameOver Text
//=======================================================================
void Renderer::DisplayGameOverScreen()
{
	glColor3f(sin((float)glutGet(GLUT_ELAPSED_TIME)/350) + 0.5f, 0.f, 0.f);

	glLineWidth(5);

	RenderText("Game Over", glutGet(GLUT_WINDOW_WIDTH)/5, glutGet(GLUT_WINDOW_HEIGHT)/2, 10);

	glLineWidth(1);

	glColor3f(0.5f, 0.5f, 0.5f);

	DisplayScore();
}

//=======================================================================
// Initialize Hint Text
//=======================================================================
void Renderer::InitializeHintText()
{
	strcpy(mHintText, "Use L key to active laser weapon");
}

//=======================================================================
// Display Function
//=======================================================================
void Renderer::Display(void)
{
	mCurrentTime = glutGet(GLUT_ELAPSED_TIME);

	if(mCurrentTime - mPreviousTime >= 16)
	{
		mPreviousTime = mCurrentTime;

		mElapsedTime = mCurrentTime - mLastFrameTime;

		glClear(GL_COLOR_BUFFER_BIT);

		switch (GameManager::getInstance().GetGameState())
		{
		case GameState::GAMEPLAY:

			RenderGameObjects();

			CalculateFPS();

			DisplayFPS();

			DisplayHints();

			DisplayScore();

			break;

		case GameState::GAMEOVER:

			DisplayGameOverScreen();

			break;

		default:
			break;
		}

		glutSwapBuffers();

		mLastFrameTime = mCurrentTime;
	}

	glutPostRedisplay();	
}
