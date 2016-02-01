#include "GameManager.h"
#include "GameConfig.h"
#include "Vector2.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "Bullet.h"
#include <time.h>

GameObject* GameManager::mPlayerShip = nullptr;
Vector2 GameManager::mNormalizedDir = Vector2();
std::vector<GameObject *> GameManager::GameObjects;
int GameManager::mScore = 0;
GameState GameManager::mGameState = GameState::GAMEPLAY;

GameManager::GameManager() 
{
	srand(time(NULL));
}

GameManager::~GameManager()
{
	for (std::vector< GameObject* >::iterator it = GameObjects.begin() ; it != GameObjects.end(); ++it)
	{
		delete (*it);
	} 
	GameObjects.clear();
}

void GameManager::Initialize(GameObject* fPlayerShip)
{
	if(fPlayerShip == nullptr)
		return;

	mPlayerShip = fPlayerShip;
	Add(mPlayerShip);

	InitializeCallBacks();
}

void GameManager::InitializeCallBacks()
{
	glutPassiveMotionFunc(MouseMotion);
	glutMouseFunc		 (Mouse);
	glutKeyboardFunc	 (Keyboard);

	glutTimerFunc(1000.0/1.0, SpawnEnemies, 0);

	//Update Game Objects as fast as the frame-rate.
	glutTimerFunc(1000.0/60.0, UpdateGameObjects, 0);
}

void GameManager::Add(GameObject* fObject)
{
	GameObjects.push_back(fObject);
}

void GameManager::MouseMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	
	Vector2 dirVector = Vector2(x,y) - Vector2(GameConfig::getInstance().ScreenWidth/2, GameConfig::getInstance().ScreenHeight/2);
	
	float dirVectorMag = sqrtf((dirVector.x * dirVector.x) + (dirVector.y * dirVector.y));
	
	mNormalizedDir = dirVector/dirVectorMag;
	
	Vector2 shipUp(0,1);
	
	float dotProduct = (dirVector.x * shipUp.x) + (dirVector.y * shipUp.y);
	
	float cosAngle = dotProduct / (shipUp.Magnitude() * dirVector.Magnitude()) ;
	
	float angle = acos(cosAngle) * 180.f / PI;
	
	dynamic_cast<PlayerShip*>(mPlayerShip)->SetDirectionAngle( x > GameConfig::getInstance().ScreenWidth/2.f ? -angle : angle);
}

void GameManager::Mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Bullet* bullet = new Bullet(Vector2(GameConfig::getInstance().ScreenWidth/2, GameConfig::getInstance().ScreenHeight/2), mNormalizedDir);
		GameObjects.push_back(bullet);
	}
}

void GameManager::Keyboard(unsigned char key, int x, int y)
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
		//case 'l':
		//fireLaser = true;
		//	break;

	}
}

void GameManager::SpawnEnemies(int)
{
	Vector2 startlocation = GetRandomPosOffScreen();

	int level;
	(((int)startlocation.x + (int)startlocation.y) % 2) == 0 ? level = 0 : level = 1;

	EnemyShip* enemy = new EnemyShip(Vector2(startlocation.x, startlocation.y), Vector2(), 0.f, 0);
	
	GameObjects.push_back(enemy);

	float Difficulty = 1.f;

	glutTimerFunc(1000.0/Difficulty, GameManager::SpawnEnemies, 0);
}

void GameManager::UpdateGameObjects(int)
{
	if(mGameState != GameState::GAMEPLAY)
		return;

	for(unsigned int i = 0; i < GameObjects.size(); i++)
	{
		GameObject* obj = GameObjects[i];

		obj->Update(1000.0f/60.f);

		if(obj->GetType() == GameObjectType::BULLET)
		{
			if(OutofBounds(obj->GetPosition()))
				GameObjects.erase(GameObjects.begin() + i);
			else
			{
				for(GameObject* object : GameObjects)
				{
					if(EnemyShip* enemy = dynamic_cast<EnemyShip*>(object))
					{
						if(Distance(enemy->GetPosition(), obj->GetPosition()) < 40)
						{
							GameObjects.erase(GameObjects.begin() + i);
							enemy->SetDying(true);
							UpdateScore();
							break;
						}

					}
				}
			}
		}

		if(obj->GetType() == GameObjectType::ENEMY_SHIP)
		{
			if(dynamic_cast<EnemyShip*>(obj)->GetTimer() <= 0)
				GameObjects.erase(GameObjects.begin() + i);

			if(Distance(obj->GetPosition(), mPlayerShip->GetPosition()) < 40)
			{
				PlayerShip *playerShip = dynamic_cast<PlayerShip *>(mPlayerShip);
				if (NULL != playerShip)
				{
					playerShip->SetDying(true);
				}
			}
		}
	}

	//Update Game Objects as fast as the frame-rate.
	glutTimerFunc(1000.0/120.0, UpdateGameObjects, 0);
}

void GameManager::UpdateScore()
{
	mScore += glutGet(GLUT_ELAPSED_TIME)/1000;
}

Vector2 GameManager::GetRandomPosOffScreen()
{
	int side = (rand() % 4) + 1;

	int EnemySize = -10;

	switch(side)
	{
	case 1:
		return Vector2(rand() % (int)GameConfig::getInstance().ScreenWidth, -EnemySize);
		break;
	case 2:
		return Vector2(rand() % (int)GameConfig::getInstance().ScreenWidth, GameConfig::getInstance().ScreenHeight + EnemySize);
		break;
	case 3:
		return Vector2(-EnemySize, rand() % (int)GameConfig::getInstance().ScreenHeight);
		break;
	case 4:
		return Vector2(GameConfig::getInstance().ScreenWidth + EnemySize, rand() % (int)GameConfig::getInstance().ScreenHeight);
		break;
	default:
		return Vector2(0,0);
	}
}

float GameManager::Distance(Vector2 fPosition1, Vector2 fPosition2)
{
	return sqrt( (fPosition2.x - fPosition1.x)*((fPosition2.x - fPosition1.x)) + ((fPosition2.y - fPosition1.y))*((fPosition2.y - fPosition1.y)) );
}

bool GameManager::OutofBounds(Vector2 fPosition)
{
	return (fPosition.x > GameConfig::getInstance().ScreenWidth || 
		fPosition.y > GameConfig::getInstance().ScreenHeight || 
		fPosition.x < 0 || 
		fPosition.y < 0);
}
