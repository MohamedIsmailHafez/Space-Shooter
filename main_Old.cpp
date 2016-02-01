#include "TextureManager.h"
#include <glut.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

// Variables to compute frames per second
int frame;
int _timebase;
char s[50];

void timer(int);

#pragma region Definitions

#define PI 3.14159265

const int DISTANCE_DESTROY = 40;

int WIDTH  = 1280;
int HEIGHT = 720;

float enemySpeed = 2.0f;

void DebugShip(float locX, float fPosY);
void Laser();
bool fireLaser;

#pragma endregion Definitions

#pragma region Enumrators

enum TYPE
{
	ENEMY = 1,
	BULLET = 20
};

enum STATE
{
	GAMEPLAY,
	GAMEOVER
};

#pragma endregion Enumrators

#pragma region Global Variables

bool DEBUG = false;

TextureManager texManager;

bool GameOver = false;

int Score = 0, kills = 0;

float Difficulty = 1.f;

float shipRotation = 0.0f;

GLuint Player_Ship_Texture, explosionTex, enemyTexI, enemyTexII;

STATE GameState;

float elapsedTimeLastFrame, lastFrame;

int current_time = 0, previous_time = 0;

#pragma endregion Global Variables

#pragma region Classes
class Vector
{
public:
	Vector() {}
	Vector(float _x, float _y) : x(_x), y(_y) {}
	float x, y;

	Vector operator-(const Vector& v)
	{
		Vector vec;
		vec.x = this->x - v.x; 
		vec.y = this->y - v.y; 
		return vec;
	}

	Vector operator/(float value)
	{
		Vector vec;
		vec.x = this->x / value; 
		vec.y = this->y / value; 
		return vec;
	}

	Vector operator*(float value)
	{
		Vector vec;
		vec.x = this->x * value; 
		vec.y = this->y * value; 
		return vec;
	}

	void operator+=(const Vector& v)
	{
		this->x += v.x;
		this->y += v.y;
	}

	float Magnitude() { return sqrtf((x * x) + (y * y)); }

};

class Entity
{
public:
	Vector position, direction;
	TYPE type;

	Entity(float _x, float _y, Vector _direction, TYPE _type) 
	{
		type = _type;
		position.x = _x; 
		position.y = _y;
		direction = _direction;
	}
};

class Enemy : public Entity
{
public:
	Vector startPosition;
	float dirAngle;
	float timer;
	bool dying;
	int level;
	
	Enemy(float _x, float _y, Vector _direction, TYPE _type, Vector _startPosition, int _level) : Entity(_x, _y, _direction, _type)
	{
		startPosition = _startPosition;

		Vector dirVector = Vector(WIDTH/2, HEIGHT/2) - Vector(startPosition.x, startPosition.y);

		Vector normalizedDir = dirVector/dirVector.Magnitude();

		float dotProduct = (dirVector.x * 0) + (dirVector.y * 1);

		float cosAngle = dotProduct / (1 * dirVector.Magnitude()) ;

		float angle = acos(cosAngle) * 180.f / PI;

		startPosition.x < WIDTH/2 ? dirAngle = -angle : dirAngle = angle;

		direction = normalizedDir;

		timer = 1.0f;

		dying = false;

		level = _level;
	}
};

class Entities
{
private:
	static const int Speed = 1;
public:
	std::vector<Entity*> entities;

	void Render(float dt)
	{
		for(unsigned int i=0; i<entities.size(); i++)
		{
			glLoadIdentity();

			Entity* e = entities[i];
			
			switch(e->type)
			{
				case BULLET:
				{
					e->position += e->direction * Speed/10 * e->type * dt;

					glBegin(GL_POINTS);
					glVertex2f(e->position.x, e->position.y);
					glEnd();
				}
				break;

				case ENEMY:
				{
					Enemy* enemy = static_cast<Enemy*>(e);

					enemy->position += enemy->direction * enemySpeed/10 * dt;

					//Debug
					//cout << enemy->position.x << " , " << enemy->position.y << endl;

					if(enemy->dying)
					{
						texManager.DrawTextured(enemy->position.x, enemy->position.y, 60.0f, 60.0f, explosionTex);
					}
					else
					{
						glTranslatef(enemy->position.x, enemy->position.y, 0);
						glRotatef(enemy->dirAngle, 0 ,0, 1);
						if(enemy->level == 1)
						glScalef(.5,.5,0);
						

						if(DEBUG)
						{
							glColor3f(1,0,0);
							DebugShip(0, 0);
						}
						else
						{
							if(enemy->level == 1)
								texManager.DrawTextured(0, 0, 60.0f, 60.0f, enemyTexI);
							else
								texManager.DrawTextured(0, 0, 60.0f, 60.0f, enemyTexII);
						}
					}
				}
				break;

			}

		}
	}

	void AddEntity(Entity *entity)
	{
		entities.push_back(entity);
	}

	float Distance(float dX0, float dY0, float dX1, float dY1)
	{
		return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
	}

	bool OutofBounds(Vector position)
	{
		return (position.x > WIDTH || position.y > HEIGHT || position.x < 0 || position.y < 0);
	}

	void CheckCollision(Entities* others)
	{
		for(unsigned int i=0; i<entities.size(); i++)
		{
			if(OutofBounds(entities[i]->position))
				entities.erase(entities.begin()+i);
		}

		for(unsigned int i=0; i<others->entities.size(); i++)
		{
			if(OutofBounds(others->entities[i]->position))
				others->entities.erase(others->entities.begin()+i);
		}

		for(unsigned int i=0; i<others->entities.size(); i++)
		{
			Vector pos = others->entities[i]->position;

			if(Distance(pos.x, pos.y, WIDTH/2, HEIGHT/2) < DISTANCE_DESTROY)
			{
				GameOver = true;

				others->entities.erase(others->entities.begin()+i);
			}
		}

		for(unsigned int i=0; i<others->entities.size(); i++)
		{
			Enemy* e = (Enemy*)others->entities[i];

			if(e->dying)
				e->timer -= 0.1;

			if(e->timer <= 0)
				others->entities.erase(others->entities.begin()+i);
		}

		for(unsigned int i=0; i<entities.size(); i++)
		{
			for(unsigned int j=0; j<others->entities.size(); j++)
			{
				if( Distance(entities[i]->position.x, entities[i]->position.y, others->entities[j]->position.x, others->entities[j]->position.y) < DISTANCE_DESTROY)
				{
					((Enemy*)others->entities[j])->dying = true;
					entities.erase(entities.begin()+i);
					Score += glutGet(GLUT_ELAPSED_TIME)/1000 * (((Enemy*)others->entities[j])->level + 1);
					kills++;
					if(kills % 10 ==0)
					{
						enemySpeed += 0.1;
						Difficulty += 0.1;
					}
					break;
				}
			}
		}
	}
};

#pragma endregion Classes

Entities bulletsList, EnemyList;

//=======================================================================
// Render Text
//=======================================================================
void renderText(char*string, int x, int y, int scale) 
{ 
	 char *c; 
	 glLoadIdentity();
	 glTranslatef(x, y, 0); 
	 glScalef((float)scale/10, (float)scale/10, 1); 
	
	 for (c=string; *c != '\0'; c++) 
	 { 
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN , *c); 
	 }  
}

//=======================================================================
// Render Score
//=======================================================================
void DisplayScore()
{
	glColor3f(0.6,0.6,0.6);
	std::stringstream strs;
	strs << "Score: " << Score;
	std::string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();
	renderText(char_type, WIDTH-250, HEIGHT-50, 2);
}

//=======================================================================
// Render Hint Text
//=======================================================================
void DisplayHints()
{
	glColor3f(0.3,0.3,0.3);
	std::stringstream strs;
	strs << "Use L key to active laser weapon";
	std::string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();
	renderText(char_type, WIDTH/2 - 200, 50, 1);
}
//=======================================================================
// Debug Ship Function
//=======================================================================
void DebugShip(float locX, float fPosY)
{
	float size = 20.f;
	glTranslatef(-locX, -fPosY, 0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(locX, fPosY + 2.0f*size);
		glVertex2f(locX + size, fPosY - size);
		glVertex2f(locX - size, fPosY - size);
	glEnd();
}

//=======================================================================
// Ship
//=======================================================================
void Ship(int locX, int fPosY, float rotation)
{
	glLoadIdentity();

	glTranslatef(WIDTH/2, HEIGHT/2, 0);
	glRotatef(rotation, 0, 0, 1);
	
	if(!GameOver)
	{
		if(DEBUG)
			DebugShip(locX, fPosY);
		else		
			texManager.DrawTextured(0, 0, 64, 64, Player_Ship_Texture);
	}
	else
	{
		texManager.DrawTextured(0, 0, 512, 512, explosionTex);
		GameState = GAMEOVER;
	}
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void Init(void)
{
    glClearColor(0.0,0.0,0.0,0.0); 

	glPointSize(4);
 
    glMatrixMode(GL_PROJECTION); 
     
    glLoadIdentity(); 
 
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);  
 
    glMatrixMode(GL_MODELVIEW); 
     
    glLoadIdentity(); 
}

//=======================================================================
// Reshape Function
//=======================================================================
void Reshape(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, width, height);	//Set Viewport Size and start position

	glMatrixMode(GL_PROJECTION);
     
    glLoadIdentity();   

    gluOrtho2D(0.0, width, 0.0, height); 

	glMatrixMode(GL_MODELVIEW); 
     
    glLoadIdentity(); 
}

//=======================================================================
// Draw Background
//=======================================================================
void Background()
{
	//glBindTexture(GL_TEXTURE_2D, backgroundTex);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f( 0,0);
		glTexCoord2f(1, 0); glVertex2f( WIDTH, 0);
		glTexCoord2f(1, 1); glVertex2f( WIDTH, HEIGHT);
		glTexCoord2f(0, 1); glVertex2f( 0,HEIGHT);
	glEnd();
}

//=======================================================================
// FPS Function
//=======================================================================
void CalculateFPS()
{
	// Code to compute frames per second
	frame++;
	glColor3f(0.2, 0.2, 0.2);
	if (current_time - _timebase > 1000) {
		sprintf(s,"FPS %1.0f", frame*1000.0/(current_time-_timebase));
		_timebase = current_time;
		frame = 0;
	}
}

//=======================================================================
// Display FPS Function
//=======================================================================
void DisplayFPS()
{
	glPushMatrix();
	renderText(s, 5, glutGet(GLUT_WINDOW_HEIGHT)-20, 1);
	glPopMatrix();
}

//=======================================================================
// Display Function
//=======================================================================
void Display(void)
{
	current_time = glutGet(GLUT_ELAPSED_TIME);

	if(current_time - previous_time > 15)
	{
		previous_time = current_time;

		elapsedTimeLastFrame =  current_time - lastFrame;

		glClear(GL_COLOR_BUFFER_BIT); 

		switch(GameState)	
		{
		case GAMEOVER:

			glColor3f(1.0, 0.0, 0.0);
			
			glLineWidth(20);
			renderText("GAME OVER", WIDTH/5, HEIGHT/2, 10);
			
			glLineWidth(1);
			DisplayScore();

			break;

		case GAMEPLAY:
		
			glColor3f(1.0, 1.0, 1.0);

			Ship(WIDTH/2, HEIGHT/2, shipRotation);

			EnemyList.Render(elapsedTimeLastFrame);
		
			glColor3f(1,0,0);
			bulletsList.Render(elapsedTimeLastFrame);

			if(fireLaser)
			Laser();
		
			DisplayScore();
			DisplayHints();
		
			CalculateFPS();
			DisplayFPS();
		
			break;
		}

		/* update animation */
		bulletsList.CheckCollision(&EnemyList);
		
		lastFrame = current_time;

	    glutSwapBuffers();

	}

	glutPostRedisplay();
}

//=======================================================================
// Random Enemy Location Function
//=======================================================================
Vector GetRandomPosOffScreen()
{
	int side = (rand() % 4) + 1;

	int EnemySize = -10;

	switch(side)
	{
	case 1:
		return Vector(rand() % WIDTH, -EnemySize);
		break;
	case 2:
		return Vector(rand() % WIDTH, HEIGHT + EnemySize);
		break;
	case 3:
		return Vector(-EnemySize, rand() % HEIGHT);
		break;
	case 4:
		return Vector(WIDTH+EnemySize, rand() % HEIGHT);
		break;
	default:
		return Vector(0,0);
	}
}

//=======================================================================
// Random Enemy Location Function
//=======================================================================
void SpawnEnemies(int)
{
	Vector startlocation = GetRandomPosOffScreen();
	int level;
	(((int)startlocation.x + (int)startlocation.y) % 2) == 0 ? level = 0 : level = 1;
	Enemy *enemy = new Enemy(startlocation.x, startlocation.y, Vector(), ENEMY, startlocation, level);
	EnemyList.AddEntity(enemy);

	glutTimerFunc(1000.0/Difficulty, SpawnEnemies, 0);
}

//=======================================================================
// Mouse Function
//=======================================================================
void Mouse(int button, int state, int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Vector dirVector = Vector(x,y) - Vector(WIDTH/2, HEIGHT/2);

		float dirVectorMag = sqrtf((dirVector.x * dirVector.x) + (dirVector.y * dirVector.y));

		Vector normalizedDir = dirVector/dirVectorMag;

		Entity *bullet = new Entity(WIDTH/2, HEIGHT/2, normalizedDir, BULLET);

		bulletsList.AddEntity(bullet);

		Vector shipUp(0,1);

		float dotProduct = (dirVector.x * shipUp.x) + (dirVector.y * shipUp.y);

		float cosAngle = dotProduct / (shipUp.Magnitude() * dirVector.Magnitude()) ;

		float angle = acos(cosAngle) * 180.f / PI;

		x > WIDTH/2 ? shipRotation = -angle : shipRotation = angle;
	}
}

//=======================================================================
// Mouse Motion Function
//=======================================================================
void MouseMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	Vector dirVector = Vector(x,y) - Vector(WIDTH/2, HEIGHT/2);

	float dirVectorMag = sqrtf((dirVector.x * dirVector.x) + (dirVector.y * dirVector.y));

	Vector normalizedDir = dirVector/dirVectorMag;

	Vector shipUp(0,1);

	float dotProduct = (dirVector.x * shipUp.x) + (dirVector.y * shipUp.y);

	float cosAngle = dotProduct / (shipUp.Magnitude() * dirVector.Magnitude()) ;

	float angle = acos(cosAngle) * 180.f / PI;

	x > WIDTH/2 ? shipRotation = -angle : shipRotation = angle;
	
	/*//Debug Vector Direction
	glLoadIdentity();
	glBegin(GL_LINES);
	glVertex2i(WIDTH/2, HEIGHT/2);
	glVertex2i(x,y);
	glEnd();
	glFlush();*/	
}

//=======================================================================
// Keyboard Function
//=======================================================================
void Keyboard(unsigned char key, int x, int y)
{
		switch(key)
		{
			case 27:
				exit(0);
				break;

			case 'f':
				glutFullScreen();
				break;

			case 'b':
				glutReshapeWindow(1280,720);
				glutPositionWindow(100,150);
				break;
			case 'l':
				fireLaser = true;
				break;

		}
}

//=======================================================================
// Timer Function
//=======================================================================
void timer(int)
{
    /* update animation */
	CalculateFPS();
	
	bulletsList.CheckCollision(&EnemyList);

    glutPostRedisplay();
    glutTimerFunc(1000.f/60.f, timer, 0);
}

//=======================================================================
// Clean Up Function
//=======================================================================
void OnExit(void)
{
	bulletsList.entities.clear();

	EnemyList.entities.clear();

	glDeleteTextures(1, &Player_Ship_Texture);
	glDeleteTextures(1, &explosionTex);
	glDeleteTextures(1, &enemyTexI);
	glDeleteTextures(1, &enemyTexII);
}

//=======================================================================
// Load Textures Function
//=======================================================================
void LoadTextures()
{
	Player_Ship_Texture		= texManager.LoadTexture("Alpha.bmp", true);
	explosionTex	= texManager.LoadTexture("explosion.bmp", false);
	enemyTexI		= texManager.LoadTexture("ship6_0.bmp", true);
	enemyTexII		= texManager.LoadTexture("enemyII.bmp", true);
}

//=======================================================================
// Laser Function
//=======================================================================
void Laser()
{
	glLineWidth(10);
	static float Laser_x = 0;
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
	glVertex2f(Laser_x,0);
	glVertex2f(Laser_x,HEIGHT);
	glEnd();
	glPopMatrix();

	Laser_x += 25;

	for(unsigned int i = 0; i < EnemyList.entities.size(); i++)
	{
		if(abs(EnemyList.entities.at(i)->position.x - Laser_x) < 20)
			((Enemy*)EnemyList.entities.at(i))->dying = true;
	}
	glLineWidth(1);

	if(Laser_x >= WIDTH) 
	{
		Laser_x = 0;
		fireLaser = false;
	}
}
////=======================================================================
//// Main Function
////=======================================================================
//void main(int argc, char** argv)
//{
//    glutInit(&argc, argv);  
//     
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
// 
//    glutInitWindowSize(WIDTH,HEIGHT);  
//
//    glutInitWindowPosition(100, 150); 
// 
//    glutCreateWindow("Space Shooter"); 
// 
//	//Register Callbacks
//    glutDisplayFunc(Display); 
//	glutReshapeFunc(Reshape);
//	glutMouseFunc(Mouse);
//	glutPassiveMotionFunc(MouseMotion);
//	glutKeyboardFunc(Keyboard);
// 
//    Init(); `
//
//	LoadTextures();
//
//	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
//
//	//glutTimerFunc(1000.0/60.0, timer, 0);
//
//	glutTimerFunc(1000.0/1.0, SpawnEnemies, 0);
//
//	srand(time(NULL));
//
//	atexit(OnExit);
//
//	GameState = GAMEPLAY;
//
//    glutMainLoop(); 
//}