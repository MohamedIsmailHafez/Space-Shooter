#include "EnemyShip.h"
#include "GameConfig.h"

#include <math.h>

EnemyShip::EnemyShip(const Vector2& fPosition, const Vector2& fDirection, float fDirectionAngle, int fLevel)
	: Ship(fPosition, fDirection, fDirectionAngle, GameObjectType::ENEMY_SHIP), 
	  mLevel(fLevel)
{
	Vector2 dirVector = Vector2((GameConfig::getInstance().ScreenWidth)/2.0f, (GameConfig::getInstance().ScreenHeight)/2.0f) - mPosition;

	dirVector.Normalize();

	float dotProduct = (dirVector.x * 0) + (dirVector.y * 1);

	float cosAngle = dotProduct / (1 * dirVector.Magnitude()) ;

	float angle = acos(cosAngle) * 180.f / PI;

	mPosition.x < ((float)GameConfig::getInstance().ScreenWidth)/2.0f ? mDirectionangle = -angle : mDirectionangle = angle;

	mDirection = dirVector;
}

EnemyShip::~EnemyShip()
{

}

//=======================================================================
// OpengL Render Function
//=======================================================================
void EnemyShip::GLRender()
{
	glLoadIdentity();

	glTranslatef(mPosition.x, mPosition.y, 0);

	if(mIsDying)
	{
		GameTextureManager::getInstance().DrawTextured(0, 0, 60.0f, 60.0f, GameTextureManager::getInstance().explosionTex);
	}
	else
	{
		glRotatef(mDirectionangle, 0 ,0, 1);
		if(mLevel == 1)
			glScalef(.5,.5,0);

#if DEBUG
		glColor3f(1,0,0);
		RenderDebugShip(0,0);
#else
		if(mLevel == 1)
			GameTextureManager::getInstance().DrawTextured(0, 0, 60.0f, 60.0f, GameTextureManager::getInstance().enemyTexI);
		else
			GameTextureManager::getInstance().DrawTextured(0, 0, 60.0f, 60.0f, GameTextureManager::getInstance().enemyTexII);
#endif	
}
}

//=======================================================================
// Update Function
//=======================================================================
void EnemyShip::Update(float fDeltaTime)
{
	mPosition += mDirection * 200.f * float(fDeltaTime/1000.f);

	if(mIsDying)
		mTimer -= 0.06f;
}